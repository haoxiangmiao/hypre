/*BHEADER**********************************************************************
 * Copyright (c) 2008,  Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 * This file is part of HYPRE.  See file COPYRIGHT for details.
 *
 * HYPRE is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License (as published by the Free
 * Software Foundation) version 2.1 dated February 1999.
 *
 * $Revision$
 ***********************************************************************EHEADER*/

/******************************************************************************
 *
 * Member functions for hypre_ParCompGrid and hypre_ParCompGridCommPkg classes.
 *
 *****************************************************************************/

#include "_hypre_parcsr_ls.h"
#include "_hypre_utilities.h"
#include <stdio.h>
#include <math.h>

HYPRE_Int
RecursivelyMarkGhostDofs(HYPRE_Int node, HYPRE_Int m, hypre_ParCompGrid *compGrid);

hypre_ParCompGrid *
hypre_ParCompGridCreate ()
{
   hypre_ParCompGrid      *compGrid;

   compGrid = hypre_CTAlloc(hypre_ParCompGrid, 1, HYPRE_MEMORY_HOST);

   hypre_ParCompGridNumNodes(compGrid) = 0;
   hypre_ParCompGridNumOwnedBlocks(compGrid) = 0;
   hypre_ParCompGridOwnedBlockStarts(compGrid) = NULL;
   hypre_ParCompGridNumRealNodes(compGrid) = 0;
   hypre_ParCompGridNumCPoints(compGrid) = 0;
   hypre_ParCompGridMemSize(compGrid) = 0;
   hypre_ParCompGridAMemSize(compGrid) = 0;
   hypre_ParCompGridPMemSize(compGrid) = 0;   
   hypre_ParCompGridU(compGrid) = NULL;
   hypre_ParCompGridF(compGrid) = NULL;
   hypre_ParCompGridT(compGrid) = NULL;
   hypre_ParCompGridS(compGrid) = NULL;
   hypre_ParCompGridTemp(compGrid) = NULL;
   hypre_ParCompGridTemp2(compGrid) = NULL;
   hypre_ParCompGridL1Norms(compGrid) = NULL;
   hypre_ParCompGridCFMarkerArray(compGrid) = NULL;
   hypre_ParCompGridCMask(compGrid) = NULL;
   hypre_ParCompGridFMask(compGrid) = NULL;
   hypre_ParCompGridChebyCoeffs(compGrid) = NULL;
   hypre_ParCompGridGlobalIndices(compGrid) = NULL;
   hypre_ParCompGridCoarseGlobalIndices(compGrid) = NULL;
   hypre_ParCompGridCoarseLocalIndices(compGrid) = NULL;
   hypre_ParCompGridRealDofMarker(compGrid) = NULL;

   hypre_ParCompGridARowPtr(compGrid) = NULL;
   hypre_ParCompGridAColInd(compGrid) = NULL;
   hypre_ParCompGridAGlobalColInd(compGrid) = NULL;
   hypre_ParCompGridAData(compGrid) = NULL;
   hypre_ParCompGridPRowPtr(compGrid) = NULL;
   hypre_ParCompGridPColInd(compGrid) = NULL;
   hypre_ParCompGridPData(compGrid) = NULL;

   hypre_ParCompGridA(compGrid) = NULL;
   hypre_ParCompGridAReal(compGrid) = NULL;
   hypre_ParCompGridP(compGrid) = NULL;
   hypre_ParCompGridR(compGrid) = NULL;

   return compGrid;
}

HYPRE_Int
hypre_ParCompGridDestroy ( hypre_ParCompGrid *compGrid )
{
   
   if (hypre_ParCompGridOwnedBlockStarts(compGrid))
   {
      hypre_TFree(hypre_ParCompGridOwnedBlockStarts(compGrid), HYPRE_MEMORY_HOST);
   }

   if (hypre_ParCompGridL1Norms(compGrid))
   {
      hypre_TFree(hypre_ParCompGridL1Norms(compGrid), HYPRE_MEMORY_SHARED);
   }

   if (hypre_ParCompGridCFMarkerArray(compGrid))
   {
      hypre_TFree(hypre_ParCompGridCFMarkerArray(compGrid), HYPRE_MEMORY_SHARED);
   }

   if (hypre_ParCompGridCMask(compGrid))
   {
      hypre_TFree(hypre_ParCompGridCMask(compGrid), HYPRE_MEMORY_SHARED);
   }

   if (hypre_ParCompGridFMask(compGrid))
   {
      hypre_TFree(hypre_ParCompGridFMask(compGrid), HYPRE_MEMORY_SHARED);
   }

   if (hypre_ParCompGridChebyCoeffs(compGrid))
   {
      hypre_TFree(hypre_ParCompGridChebyCoeffs(compGrid), HYPRE_MEMORY_SHARED);
   }

   if (hypre_ParCompGridU(compGrid))
   {
      hypre_SeqVectorDestroy(hypre_ParCompGridU(compGrid));
   }

   if (hypre_ParCompGridF(compGrid))
   {
      hypre_SeqVectorDestroy(hypre_ParCompGridF(compGrid));
   }

   if (hypre_ParCompGridT(compGrid))
   {
      hypre_SeqVectorDestroy(hypre_ParCompGridT(compGrid));
   }

   if (hypre_ParCompGridS(compGrid))
   {
      hypre_SeqVectorDestroy(hypre_ParCompGridS(compGrid));
   }

   if (hypre_ParCompGridTemp(compGrid))
   {
      hypre_SeqVectorDestroy(hypre_ParCompGridTemp(compGrid));
   }

   if (hypre_ParCompGridTemp2(compGrid))
   {
      hypre_SeqVectorDestroy(hypre_ParCompGridTemp2(compGrid));
   }

   if (hypre_ParCompGridGlobalIndices(compGrid))
   {
      hypre_TFree(hypre_ParCompGridGlobalIndices(compGrid), HYPRE_MEMORY_HOST);
   }
   
   if (hypre_ParCompGridCoarseGlobalIndices(compGrid))
   {
      hypre_TFree(hypre_ParCompGridCoarseGlobalIndices(compGrid), HYPRE_MEMORY_HOST);
   }

   if (hypre_ParCompGridCoarseLocalIndices(compGrid))
   {
      hypre_TFree(hypre_ParCompGridCoarseLocalIndices(compGrid), HYPRE_MEMORY_HOST);
   }

   if (hypre_ParCompGridA(compGrid))
   {
      hypre_CSRMatrixDestroy(hypre_ParCompGridA(compGrid));
   }

   if (hypre_ParCompGridP(compGrid))
   {
      hypre_CSRMatrixDestroy(hypre_ParCompGridP(compGrid));
   }

   if (hypre_ParCompGridR(compGrid))
   {
      hypre_CSRMatrixDestroy(hypre_ParCompGridR(compGrid));
   }

   hypre_TFree(compGrid, HYPRE_MEMORY_HOST);   
   

   return 0;
}

HYPRE_Int
hypre_ParCompGridInitialize ( hypre_ParAMGData *amg_data, HYPRE_Int padding, HYPRE_Int level )
{
   HYPRE_Int      myid;
   hypre_MPI_Comm_rank(hypre_MPI_COMM_WORLD, &myid );

   HYPRE_Int         i,j;

   // Get info from the amg data structure
   hypre_ParCompGrid *compGrid = hypre_ParAMGDataCompGrid(amg_data)[level];
   hypre_ParVector *residual = hypre_ParAMGDataFArray(amg_data)[level];
   HYPRE_Int *CF_marker_array = hypre_ParAMGDataCFMarkerArray(amg_data)[level];
   hypre_ParCSRMatrix *A = hypre_ParAMGDataAArray(amg_data)[level];
   hypre_ParCSRMatrix *P = NULL;
   HYPRE_Int coarseStart = 0;
   if (level != hypre_ParAMGDataNumLevels(amg_data) - 1)
   {
      P = hypre_ParAMGDataPArray(amg_data)[level];
      coarseStart = hypre_ParVectorFirstIndex(hypre_ParAMGDataFArray(amg_data)[level+1]);
   }

   hypre_Vector *residual_local = hypre_ParVectorLocalVector(residual);
   HYPRE_Int         num_nodes = hypre_VectorSize(residual_local);
   HYPRE_Int         mem_size = num_nodes + 2 * (padding + hypre_ParAMGDataAMGDDNumGhostLayers(amg_data)) * hypre_CSRMatrixNumCols( hypre_ParCSRMatrixOffd(A) );
   HYPRE_Real        over_allocation_factor = (HYPRE_Real) mem_size;
   if (num_nodes > 0) over_allocation_factor = ((HYPRE_Real) mem_size) / ((HYPRE_Real) num_nodes);

   hypre_ParCompGridNumNodes(compGrid) = num_nodes;
   hypre_ParCompGridNumOwnedBlocks(compGrid) = 1;
   hypre_ParCompGridOwnedBlockStarts(compGrid) = hypre_CTAlloc(HYPRE_Int, 2, HYPRE_MEMORY_HOST);
   hypre_ParCompGridOwnedBlockStarts(compGrid)[0] = 0;
   hypre_ParCompGridOwnedBlockStarts(compGrid)[1] = num_nodes;
   hypre_ParCompGridNumRealNodes(compGrid) = num_nodes;
   hypre_ParCompGridMemSize(compGrid) = mem_size;

   HYPRE_Int A_nnz = hypre_CSRMatrixNumNonzeros( hypre_ParCSRMatrixDiag(A) ) + hypre_CSRMatrixNumNonzeros( hypre_ParCSRMatrixOffd(A) );
   hypre_ParCompGridAMemSize(compGrid) = over_allocation_factor*A_nnz;
   HYPRE_Int P_nnz;
   if (P)
   {
      P_nnz = hypre_CSRMatrixNumNonzeros( hypre_ParCSRMatrixDiag(P) ) + hypre_CSRMatrixNumNonzeros( hypre_ParCSRMatrixOffd(P) );
      hypre_ParCompGridPMemSize(compGrid) = over_allocation_factor*P_nnz;
   }

   // Allocate space for the info on the comp nodes
   HYPRE_Int        *global_indices_comp = hypre_CTAlloc(HYPRE_Int, mem_size, HYPRE_MEMORY_HOST);
   HYPRE_Int        *real_dof_marker = hypre_CTAlloc(HYPRE_Int, mem_size, HYPRE_MEMORY_HOST);
   HYPRE_Int        *coarse_global_indices_comp = NULL; 
   HYPRE_Int        *coarse_local_indices_comp = NULL;
   if ( CF_marker_array )
   {
      coarse_global_indices_comp = hypre_CTAlloc(HYPRE_Int, mem_size, HYPRE_MEMORY_HOST); 
      coarse_local_indices_comp = hypre_CTAlloc(HYPRE_Int, mem_size, HYPRE_MEMORY_HOST);
   }
   HYPRE_Int        *A_rowptr = hypre_CTAlloc(HYPRE_Int, mem_size+1, HYPRE_MEMORY_HOST);
   HYPRE_Int        *A_colind = hypre_CTAlloc(HYPRE_Int, hypre_ParCompGridAMemSize(compGrid), HYPRE_MEMORY_HOST);
   HYPRE_Int        *A_global_colind = hypre_CTAlloc(HYPRE_Int, hypre_ParCompGridAMemSize(compGrid), HYPRE_MEMORY_HOST);
   HYPRE_Complex    *A_data = hypre_CTAlloc(HYPRE_Complex, hypre_ParCompGridAMemSize(compGrid), HYPRE_MEMORY_HOST);
   HYPRE_Int        *P_rowptr = NULL;
   HYPRE_Int        *P_colind = NULL;
   HYPRE_Complex    *P_data = NULL;
   if (P)
   {
      P_rowptr = hypre_CTAlloc(HYPRE_Int, mem_size+1, HYPRE_MEMORY_HOST);
      P_colind = hypre_CTAlloc(HYPRE_Int, hypre_ParCompGridPMemSize(compGrid), HYPRE_MEMORY_HOST);
      P_data = hypre_CTAlloc(HYPRE_Complex, hypre_ParCompGridPMemSize(compGrid), HYPRE_MEMORY_HOST);
   }

   // Set up temporary arrays for getting rows of matrix A
   HYPRE_Int         row_size;
   HYPRE_Int         *row_col_ind;
   HYPRE_Complex     *row_values;

   // Initialize composite grid data to the given information
   HYPRE_Int        coarseIndexCounter = 0;

   for (i = 0; i < num_nodes; i++)
   {
      global_indices_comp[i] = hypre_ParVectorFirstIndex(residual) + i;
      real_dof_marker[i] = 1;
      if ( CF_marker_array ) // if there is a CF_marker_array for this level (i.e. unless we are on the coarsest level)
      {
         if ( CF_marker_array[i] == 1 )
         {
            coarse_global_indices_comp[i] = coarseIndexCounter + coarseStart;
            coarse_local_indices_comp[i] = coarseIndexCounter;
            coarseIndexCounter++;
         }
         else 
         {
               coarse_global_indices_comp[i] = -1;
               coarse_local_indices_comp[i] = -1;
         }
      }
      else coarse_global_indices_comp = coarse_local_indices_comp = NULL;
      
      // Setup row of matrix A
      hypre_ParCSRMatrixGetRow( A, global_indices_comp[i], &row_size, &row_col_ind, &row_values );
      A_rowptr[i+1] = A_rowptr[i] + row_size;
      for (j = A_rowptr[i]; j < A_rowptr[i+1]; j++)
      {
         A_data[j] = row_values[j - A_rowptr[i]];

         HYPRE_Int global_index = row_col_ind[j - A_rowptr[i]];
         A_global_colind[j] = global_index;

         if ( global_index >= hypre_ParVectorFirstIndex(residual) && global_index <= hypre_ParVectorLastIndex(residual) )
            A_colind[j] = global_index - hypre_ParVectorFirstIndex(residual);
         else A_colind[j] = -1;
      }
      hypre_ParCSRMatrixRestoreRow( A, i, &row_size, &row_col_ind, &row_values );

      if (P)
      {
         // Setup row of matrix P
         hypre_ParCSRMatrixGetRow( P, global_indices_comp[i], &row_size, &row_col_ind, &row_values );
         P_rowptr[i+1] = P_rowptr[i] + row_size;
         for (j = P_rowptr[i]; j < P_rowptr[i+1]; j++)
         {
            P_data[j] = row_values[j - P_rowptr[i]];
            P_colind[j] = row_col_ind[j - P_rowptr[i]];
         }
         hypre_ParCSRMatrixRestoreRow( P, i, &row_size, &row_col_ind, &row_values );
      }
   }

   // Set attributes for compGrid
   hypre_ParCompGridGlobalIndices(compGrid) = global_indices_comp;
   hypre_ParCompGridRealDofMarker(compGrid) = real_dof_marker;
   hypre_ParCompGridCoarseGlobalIndices(compGrid) = coarse_global_indices_comp;
   hypre_ParCompGridCoarseLocalIndices(compGrid) = coarse_local_indices_comp;
   hypre_ParCompGridARowPtr(compGrid) = A_rowptr;
   hypre_ParCompGridAColInd(compGrid) = A_colind;
   hypre_ParCompGridAGlobalColInd(compGrid) = A_global_colind;
   hypre_ParCompGridAData(compGrid) = A_data;
   hypre_ParCompGridPRowPtr(compGrid) = P_rowptr;
   hypre_ParCompGridPColInd(compGrid) = P_colind;
   hypre_ParCompGridPData(compGrid) = P_data;

   return 0;
}

HYPRE_Int 
hypre_ParCompGridSetupRelax( hypre_ParAMGData *amg_data )
{
   HYPRE_Int level, i, j;

   for (level = 0; level < hypre_ParAMGDataNumLevels(amg_data); level++)
   {
      hypre_ParCompGrid *compGrid = hypre_ParAMGDataCompGrid(amg_data)[level];

      if (hypre_ParAMGDataFACRelaxType(amg_data) == 2)
      {
         // Setup chebyshev coefficients
         hypre_CSRMatrix *A = hypre_ParCompGridA(compGrid);
         HYPRE_Real    *coefs = hypre_ParCompGridChebyCoeffs(compGrid);
         HYPRE_Int     scale = hypre_ParAMGDataChebyScale(amg_data);
         HYPRE_Int     order = hypre_ParAMGDataChebyOrder(amg_data);

         // Select submatrix of real to real connections
         HYPRE_Int nnz = 0;
         for (i = 0; i < hypre_ParCompGridNumRealNodes(compGrid); i++)
         {
            for (j = hypre_CSRMatrixI(A)[i]; j < hypre_CSRMatrixI(A)[i+1]; j++)
            {
               if (hypre_CSRMatrixJ(A)[j] < hypre_ParCompGridNumRealNodes(compGrid)) nnz++;
            }
         }
         HYPRE_Int *A_real_i = hypre_CTAlloc(HYPRE_Int, hypre_ParCompGridNumRealNodes(compGrid)+1, HYPRE_MEMORY_SHARED);
         HYPRE_Int *A_real_j = hypre_CTAlloc(HYPRE_Int, nnz, HYPRE_MEMORY_SHARED);
         HYPRE_Complex *A_real_data = hypre_CTAlloc(HYPRE_Complex, nnz, HYPRE_MEMORY_SHARED);
         nnz = 0;
         for (i = 0; i < hypre_ParCompGridNumRealNodes(compGrid); i++)
         {
            for (j = hypre_CSRMatrixI(A)[i]; j < hypre_CSRMatrixI(A)[i+1]; j++)
            {
               if (hypre_CSRMatrixJ(A)[j] < hypre_ParCompGridNumRealNodes(compGrid))
               {
                  A_real_j[nnz] = hypre_CSRMatrixJ(A)[j];
                  A_real_data[nnz] = hypre_CSRMatrixData(A)[j];
                  nnz++;
               }
            }
            A_real_i[i+1] = nnz;
         }

         HYPRE_BigInt *row_starts = hypre_CTAlloc(HYPRE_BigInt, 2, HYPRE_MEMORY_HOST);
         row_starts[0] = 0;
         row_starts[1] = hypre_ParCompGridNumRealNodes(compGrid);
         hypre_ParCSRMatrix *A_real = hypre_ParCSRMatrixCreate( MPI_COMM_SELF,
                             (HYPRE_BigInt) hypre_ParCompGridNumRealNodes(compGrid),
                             (HYPRE_BigInt) hypre_ParCompGridNumRealNodes(compGrid),
                             row_starts,
                             NULL,
                             0,
                             nnz,
                             0 );
         hypre_CSRMatrixI(hypre_ParCSRMatrixDiag(A_real)) = A_real_i;
         hypre_CSRMatrixJ(hypre_ParCSRMatrixDiag(A_real)) = A_real_j;
         hypre_CSRMatrixData(hypre_ParCSRMatrixDiag(A_real)) = A_real_data;
         hypre_CSRMatrixInitialize(hypre_ParCSRMatrixOffd(A_real));
         hypre_ParCSRMatrixColMapOffd(A_real) = hypre_CTAlloc(HYPRE_BigInt, 0, HYPRE_MEMORY_HOST);

         HYPRE_Real max_eig, min_eig = 0;

         if (hypre_ParAMGDataChebyEigEst(amg_data)) hypre_ParCSRMaxEigEstimateCG(A_real, scale, hypre_ParAMGDataChebyEigEst(amg_data), &max_eig, &min_eig);
         else hypre_ParCSRMaxEigEstimate(A_real, scale, &max_eig);

         HYPRE_Real *dummy_ptr;
         hypre_ParCSRRelax_Cheby_Setup(hypre_ParAMGDataAArray(amg_data)[level], 
                               max_eig,      
                               min_eig,     
                               hypre_ParAMGDataChebyFraction(amg_data),   
                               order,
                               0,
                               hypre_ParAMGDataChebyVariant(amg_data),           
                               &coefs,
                               &dummy_ptr);

         hypre_ParCompGridChebyCoeffs(compGrid) = coefs;

         hypre_ParCSRMatrixDestroy(A_real);

         // Calculate diagonal scaling values 
         hypre_ParCompGridL1Norms(compGrid) = hypre_CTAlloc(HYPRE_Real, hypre_ParCompGridNumNodes(compGrid), HYPRE_MEMORY_SHARED);
         for (i = 0; i < hypre_ParCompGridNumNodes(compGrid); i++)
         {
            for (j = hypre_ParCompGridARowPtr(compGrid)[i]; j < hypre_ParCompGridARowPtr(compGrid)[i+1]; j++)
            {
               if (hypre_ParCompGridAColInd(compGrid)[j] == i)
               {
                  hypre_ParCompGridL1Norms(compGrid)[i] = 1.0/sqrt(hypre_ParCompGridAData(compGrid)[j]);
                  break;
               }
            }
         }

         // Setup temporary/auxiliary vectors
         hypre_ParCompGridTemp(compGrid) = hypre_SeqVectorCreate(hypre_ParCompGridNumRealNodes(compGrid));
         hypre_SeqVectorInitialize(hypre_ParCompGridTemp(compGrid));

         hypre_ParCompGridTemp2(compGrid) = hypre_SeqVectorCreate(hypre_ParCompGridNumNodes(compGrid));
         hypre_SeqVectorInitialize(hypre_ParCompGridTemp2(compGrid));

         hypre_ParCompGridTemp3(compGrid) = hypre_SeqVectorCreate(hypre_ParCompGridNumRealNodes(compGrid));
         hypre_SeqVectorInitialize(hypre_ParCompGridTemp3(compGrid));
      }
      if (hypre_ParAMGDataFACRelaxType(amg_data) == 3)
      {
         // Calculate l1_norms
         hypre_ParCompGridL1Norms(compGrid) = hypre_CTAlloc(HYPRE_Real, hypre_ParCompGridNumNodes(compGrid), HYPRE_MEMORY_SHARED);
         for (i = 0; i < hypre_ParCompGridNumNodes(compGrid); i++)
         {
            HYPRE_Int cf_diag = hypre_ParCompGridCFMarkerArray(compGrid)[i];
            for (j = hypre_ParCompGridARowPtr(compGrid)[i]; j < hypre_ParCompGridARowPtr(compGrid)[i+1]; j++)
            {
               if (hypre_ParCompGridCFMarkerArray(compGrid)[ hypre_ParCompGridAColInd(compGrid)[j] ] == cf_diag) 
               {
                  hypre_ParCompGridL1Norms(compGrid)[i] += fabs(hypre_ParCompGridAData(compGrid)[j]);
               }
            }
         }
         // Setup temporary/auxiliary vectors
         hypre_ParCompGridTemp(compGrid) = hypre_SeqVectorCreate(hypre_ParCompGridNumNodes(compGrid));
         hypre_SeqVectorInitialize(hypre_ParCompGridTemp(compGrid));

         #if defined(HYPRE_USING_GPU) && defined(HYPRE_USING_UNIFIED_MEMORY)
         // Setup c and f point masks
         int num_c_points = 0;
         int num_f_points = 0;
         for (i = 0; i < hypre_ParCompGridNumRealNodes(compGrid); i++) if (hypre_ParCompGridCFMarkerArray(compGrid)[i]) num_c_points++;
         num_f_points = hypre_ParCompGridNumRealNodes(compGrid) - num_c_points;
         hypre_ParCompGridNumCPoints(compGrid) = num_c_points;
         hypre_ParCompGridCMask(compGrid) = hypre_CTAlloc(int, num_c_points, HYPRE_MEMORY_SHARED);
         hypre_ParCompGridFMask(compGrid) = hypre_CTAlloc(int, num_f_points, HYPRE_MEMORY_SHARED);
         int c_cnt = 0, f_cnt = 0;
         for (i = 0; i < hypre_ParCompGridNumRealNodes(compGrid); i++)
         {
            if (hypre_ParCompGridCFMarkerArray(compGrid)[i]) hypre_ParCompGridCMask(compGrid)[c_cnt++] = i;
            else hypre_ParCompGridFMask(compGrid)[f_cnt++] = i;
         }
         #endif
      }
   }


   return 0;
}

HYPRE_Int
hypre_ParCompGridFinalize( hypre_ParCompGrid **compGrid, hypre_ParCompGridCommPkg *compGridCommPkg, HYPRE_Int num_levels, HYPRE_Int transition_level, HYPRE_Int debug )
{
   HYPRE_Int level, i, j;

   // Post process to remove -1 entries from matrices and reorder !!! Is there a more efficient way here? 
   for (level = 0; level < num_levels; level++)
   {
      HYPRE_Int num_nodes = hypre_ParCompGridNumNodes(compGrid[level]);
      HYPRE_Int num_real_nodes = 0;
      for (i = 0; i < num_nodes; i++)
      {
         if (hypre_ParCompGridRealDofMarker(compGrid[level])[i]) num_real_nodes++;
      }
      hypre_ParCompGridNumRealNodes(compGrid[level]) = num_real_nodes;
      HYPRE_Int *new_indices = hypre_CTAlloc(HYPRE_Int, num_nodes, HYPRE_MEMORY_HOST);
      HYPRE_Int real_cnt = 0;
      HYPRE_Int ghost_cnt = 0;
      for (i = 0; i < num_nodes; i++)
      {
         if (hypre_ParCompGridRealDofMarker(compGrid[level])[i])
         {
            new_indices[i] = real_cnt++;
         }
         else new_indices[i] = num_real_nodes + ghost_cnt++;
      }

      // Transform indices in send_flag and recv_map
      HYPRE_Int outer_level;
      for (outer_level = 0; outer_level < num_levels; outer_level++)
      {
         HYPRE_Int num_send_partitions = hypre_ParCompGridCommPkgNumSendPartitions(compGridCommPkg)[outer_level];
         HYPRE_Int part;
         for (part = 0; part < num_send_partitions; part++)
         {
            HYPRE_Int num_send_nodes = hypre_ParCompGridCommPkgNumSendNodes(compGridCommPkg)[outer_level][part][level];
            for (i = 0; i < num_send_nodes; i++)
            {
               if (hypre_ParCompGridCommPkgSendFlag(compGridCommPkg)[outer_level][part][level][i] >= 0)
               {
                  hypre_ParCompGridCommPkgSendFlag(compGridCommPkg)[outer_level][part][level][i] = new_indices[hypre_ParCompGridCommPkgSendFlag(compGridCommPkg)[outer_level][part][level][i]];
               }
            }
            HYPRE_Int num_recv_nodes = hypre_ParCompGridCommPkgNumRecvNodes(compGridCommPkg)[outer_level][part][level];
            for (i = 0; i < num_recv_nodes; i++)
            {
               if (hypre_ParCompGridCommPkgRecvMap(compGridCommPkg)[outer_level][part][level][i] >= 0)
               {
                  hypre_ParCompGridCommPkgRecvMap(compGridCommPkg)[outer_level][part][level][i] = new_indices[hypre_ParCompGridCommPkgRecvMap(compGridCommPkg)[outer_level][part][level][i]];
               }
            }
         }
      }

      // If global indices are still needed, transform these also
      if (debug)
      {
         HYPRE_Int *new_global_indices = hypre_CTAlloc(HYPRE_Int, num_nodes, HYPRE_MEMORY_HOST);
         for (i = 0; i < num_nodes; i++)
         {
            new_global_indices[ new_indices[i] ] = hypre_ParCompGridGlobalIndices(compGrid[level])[ i ];
         }
         hypre_TFree(hypre_ParCompGridGlobalIndices(compGrid[level]), HYPRE_MEMORY_HOST);
         hypre_ParCompGridGlobalIndices(compGrid[level]) = new_global_indices;
      }

      // Setup cf marker array in correct ordering
      hypre_ParCompGridCFMarkerArray(compGrid[level]) = hypre_CTAlloc(HYPRE_Int, hypre_ParCompGridNumNodes(compGrid[level]), HYPRE_MEMORY_SHARED);
      if (hypre_ParCompGridCoarseLocalIndices(compGrid[level]))
      {
         for (i = 0; i < hypre_ParCompGridNumNodes(compGrid[level]); i++)
         {
            if (hypre_ParCompGridCoarseLocalIndices(compGrid[level])[i] >= 0) hypre_ParCompGridCFMarkerArray(compGrid[level])[ new_indices[i] ] = 1;
         }
         hypre_TFree(hypre_ParCompGridCoarseLocalIndices(compGrid[level]), HYPRE_MEMORY_HOST);
         hypre_ParCompGridCoarseLocalIndices(compGrid[level]) = NULL;
      }

      HYPRE_Int A_nnz = hypre_ParCompGridARowPtr(compGrid[level])[num_nodes];
      HYPRE_Int *new_A_rowPtr = hypre_CTAlloc(HYPRE_Int, num_nodes+1, HYPRE_MEMORY_SHARED);
      HYPRE_Int *new_A_colInd = hypre_CTAlloc(HYPRE_Int, A_nnz, HYPRE_MEMORY_SHARED);
      HYPRE_Complex *new_A_data = hypre_CTAlloc(HYPRE_Complex, A_nnz, HYPRE_MEMORY_SHARED);

      HYPRE_Int P_nnz;
      HYPRE_Int *new_P_rowPtr;
      HYPRE_Int *new_P_colInd;
      HYPRE_Complex *new_P_data;

      if (level != num_levels-1)
      {
         P_nnz = hypre_ParCompGridPRowPtr(compGrid[level])[num_nodes];
         new_P_rowPtr = hypre_CTAlloc(HYPRE_Int, num_nodes+1, HYPRE_MEMORY_SHARED);
         new_P_colInd = hypre_CTAlloc(HYPRE_Int, P_nnz, HYPRE_MEMORY_SHARED);
         new_P_data = hypre_CTAlloc(HYPRE_Complex, P_nnz, HYPRE_MEMORY_SHARED);
      }

      HYPRE_Int A_cnt = 0;
      HYPRE_Int P_cnt = 0;
      HYPRE_Int node_cnt = 0;
      // Real nodes
      for (i = 0; i < num_nodes; i++)
      {
         if (hypre_ParCompGridRealDofMarker(compGrid[level])[i])
         {
            new_A_rowPtr[node_cnt] = A_cnt;
            for (j = hypre_ParCompGridARowPtr(compGrid[level])[i]; j < hypre_ParCompGridARowPtr(compGrid[level])[i+1]; j++)
            {
               if (hypre_ParCompGridAColInd(compGrid[level])[j] >= 0)
               {
                  new_A_colInd[A_cnt] = new_indices[ hypre_ParCompGridAColInd(compGrid[level])[j] ];
                  new_A_data[A_cnt] = hypre_ParCompGridAData(compGrid[level])[j];
                  A_cnt++;
               }
            }

            if (level != num_levels-1)
            {
               new_P_rowPtr[node_cnt] = P_cnt;
               for (j = hypre_ParCompGridPRowPtr(compGrid[level])[i]; j < hypre_ParCompGridPRowPtr(compGrid[level])[i+1]; j++)
               {
                  if (hypre_ParCompGridPColInd(compGrid[level])[j] >= 0)
                  {
                     new_P_colInd[P_cnt] = hypre_ParCompGridPColInd(compGrid[level])[j];
                     new_P_data[P_cnt] = hypre_ParCompGridPData(compGrid[level])[j];
                     P_cnt++;
                  }
               }
            }
            node_cnt++;
         }
      }
      // Ghost nodes
      for (i = 0; i < num_nodes; i++)
      {
         if (!hypre_ParCompGridRealDofMarker(compGrid[level])[i])
         {
            new_A_rowPtr[node_cnt] = A_cnt;
            for (j = hypre_ParCompGridARowPtr(compGrid[level])[i]; j < hypre_ParCompGridARowPtr(compGrid[level])[i+1]; j++)
            {
               if (hypre_ParCompGridAColInd(compGrid[level])[j] >= 0)
               {
                  new_A_colInd[A_cnt] = new_indices[hypre_ParCompGridAColInd(compGrid[level])[j]];
                  new_A_data[A_cnt] = hypre_ParCompGridAData(compGrid[level])[j];
                  A_cnt++;
               }
            }

            if (level != num_levels-1)
            {
               new_P_rowPtr[node_cnt] = P_cnt;
               for (j = hypre_ParCompGridPRowPtr(compGrid[level])[i]; j < hypre_ParCompGridPRowPtr(compGrid[level])[i+1]; j++)
               {
                  if (hypre_ParCompGridPColInd(compGrid[level])[j] >= 0)
                  {
                     new_P_colInd[P_cnt] = hypre_ParCompGridPColInd(compGrid[level])[j];
                     new_P_data[P_cnt] = hypre_ParCompGridPData(compGrid[level])[j];
                     P_cnt++;
                  }
               }
            }
            node_cnt++;
         }
      }
      new_A_rowPtr[num_nodes] = A_cnt;
      if (level != num_levels-1) new_P_rowPtr[num_nodes] = P_cnt;

      // Fix up P col indices on previous level
      if (level != 0)
      {
         for (i = 0; i < hypre_ParCompGridPRowPtr(compGrid[level-1])[ hypre_ParCompGridNumNodes(compGrid[level-1]) ]; i++)
         {
            hypre_ParCompGridPColInd(compGrid[level-1])[i] = new_indices[ hypre_ParCompGridPColInd(compGrid[level-1])[i] ];
         }
      }

      // Clean up memory, deallocate old arrays and reset pointers to new arrays
      hypre_TFree(hypre_ParCompGridARowPtr(compGrid[level]), HYPRE_MEMORY_HOST);
      hypre_TFree(hypre_ParCompGridAColInd(compGrid[level]), HYPRE_MEMORY_HOST);
      hypre_TFree(hypre_ParCompGridAData(compGrid[level]), HYPRE_MEMORY_HOST);
      hypre_ParCompGridARowPtr(compGrid[level]) = new_A_rowPtr;
      hypre_ParCompGridAColInd(compGrid[level]) = new_A_colInd;
      hypre_ParCompGridAData(compGrid[level]) = new_A_data;

      if (level != num_levels-1)
      {
         hypre_TFree(hypre_ParCompGridPRowPtr(compGrid[level]), HYPRE_MEMORY_HOST);
         hypre_TFree(hypre_ParCompGridPColInd(compGrid[level]), HYPRE_MEMORY_HOST);
         hypre_TFree(hypre_ParCompGridPData(compGrid[level]), HYPRE_MEMORY_HOST);
         hypre_ParCompGridPRowPtr(compGrid[level]) = new_P_rowPtr;
         hypre_ParCompGridPColInd(compGrid[level]) = new_P_colInd;
         hypre_ParCompGridPData(compGrid[level]) = new_P_data;
      }

      hypre_TFree(new_indices, HYPRE_MEMORY_HOST);
   }

   // Setup vectors and matrices
   HYPRE_Int total_num_nodes = 0;
   for (level = 0; level < num_levels; level++)
   {
      HYPRE_Int num_nodes = hypre_ParCompGridNumNodes(compGrid[level]);
      HYPRE_Int num_real_nodes = hypre_ParCompGridNumRealNodes(compGrid[level]);
      total_num_nodes += num_nodes;
      HYPRE_Int A_nnz = hypre_ParCompGridARowPtr(compGrid[level])[num_nodes];
      HYPRE_Int A_real_nnz = hypre_ParCompGridARowPtr(compGrid[level])[num_real_nodes];
      
      hypre_ParCompGridA(compGrid[level]) = hypre_CSRMatrixCreate(num_nodes, num_nodes, A_nnz);
      hypre_CSRMatrixI(hypre_ParCompGridA(compGrid[level])) = hypre_ParCompGridARowPtr(compGrid[level]);
      hypre_CSRMatrixJ(hypre_ParCompGridA(compGrid[level])) = hypre_ParCompGridAColInd(compGrid[level]);
      hypre_CSRMatrixData(hypre_ParCompGridA(compGrid[level])) = hypre_ParCompGridAData(compGrid[level]);

      hypre_ParCompGridAReal(compGrid[level]) = hypre_CSRMatrixCreate(num_real_nodes, num_nodes, A_real_nnz);
      hypre_CSRMatrixI(hypre_ParCompGridAReal(compGrid[level])) = hypre_ParCompGridARowPtr(compGrid[level]);
      hypre_CSRMatrixJ(hypre_ParCompGridAReal(compGrid[level])) = hypre_ParCompGridAColInd(compGrid[level]);
      hypre_CSRMatrixData(hypre_ParCompGridAReal(compGrid[level])) = hypre_ParCompGridAData(compGrid[level]);

      if (level != num_levels-1)
      {
         HYPRE_Int P_nnz = hypre_ParCompGridPRowPtr(compGrid[level])[num_nodes];
         HYPRE_Int num_nodes_c = hypre_ParCompGridNumNodes(compGrid[level+1]);
         hypre_ParCompGridP(compGrid[level]) = hypre_CSRMatrixCreate(num_nodes, num_nodes_c, P_nnz);
         hypre_CSRMatrixI(hypre_ParCompGridP(compGrid[level])) = hypre_ParCompGridPRowPtr(compGrid[level]);
         hypre_CSRMatrixJ(hypre_ParCompGridP(compGrid[level])) = hypre_ParCompGridPColInd(compGrid[level]);
         hypre_CSRMatrixData(hypre_ParCompGridP(compGrid[level])) = hypre_ParCompGridPData(compGrid[level]);

         hypre_CSRMatrixTranspose(hypre_ParCompGridP(compGrid[level]), &hypre_ParCompGridR(compGrid[level]), 1);
      }

      hypre_ParCompGridU(compGrid[level]) = hypre_SeqVectorCreate(num_nodes);
      hypre_SeqVectorInitialize(hypre_ParCompGridU(compGrid[level]));

      hypre_ParCompGridS(compGrid[level]) = hypre_SeqVectorCreate(num_nodes);
      hypre_SeqVectorInitialize(hypre_ParCompGridS(compGrid[level]));

      hypre_ParCompGridT(compGrid[level]) = hypre_SeqVectorCreate(num_nodes);
      hypre_SeqVectorInitialize(hypre_ParCompGridT(compGrid[level]));
   }

   // Allocate space for the rhs vectors, compGridF, as one big block of memory for better access when packing/unpack communication buffers
   HYPRE_Complex *f_data = hypre_CTAlloc(HYPRE_Complex, total_num_nodes, HYPRE_MEMORY_SHARED);

   total_num_nodes = 0;

   for (level = 0; level < num_levels; level++)
   {
      HYPRE_Int num_nodes = hypre_ParCompGridNumNodes(compGrid[level]);

      hypre_ParCompGridF(compGrid[level]) = hypre_SeqVectorCreate(num_nodes);
      if (level != 0) hypre_SeqVectorSetDataOwner(hypre_ParCompGridF(compGrid[level]), 0);
      hypre_VectorData(hypre_ParCompGridF(compGrid[level])) = &(f_data[total_num_nodes]);

      total_num_nodes += num_nodes;
   }

   // Clean up memory for things we don't need anymore
   for (level = 0; level < transition_level; level++)
   {
      if (hypre_ParCompGridRealDofMarker(compGrid[level]))
      {
         hypre_TFree(hypre_ParCompGridRealDofMarker(compGrid[level]), HYPRE_MEMORY_HOST);
         hypre_ParCompGridRealDofMarker(compGrid[level]) = NULL;
      }
      if (hypre_ParCompGridGlobalIndices(compGrid[level]) && !debug)
      {
         hypre_TFree(hypre_ParCompGridGlobalIndices(compGrid[level]), HYPRE_MEMORY_HOST);
         hypre_ParCompGridGlobalIndices(compGrid[level]) = NULL;
      }
      if (hypre_ParCompGridAGlobalColInd(compGrid[level]) && !debug)
      {
         hypre_TFree(hypre_ParCompGridAGlobalColInd(compGrid[level]), HYPRE_MEMORY_HOST);
         hypre_ParCompGridAGlobalColInd(compGrid[level]) = NULL;
      }
      if (hypre_ParCompGridCoarseGlobalIndices(compGrid[level]))
      {
         hypre_TFree(hypre_ParCompGridCoarseGlobalIndices(compGrid[level]), HYPRE_MEMORY_HOST);
         hypre_ParCompGridCoarseGlobalIndices(compGrid[level]) = NULL;
      }
      if (hypre_ParCompGridCoarseLocalIndices(compGrid[level]))
      {
         hypre_TFree(hypre_ParCompGridCoarseLocalIndices(compGrid[level]), HYPRE_MEMORY_HOST);
         hypre_ParCompGridCoarseLocalIndices(compGrid[level]) = NULL;
      }
   }

   return 0;
}

HYPRE_Int
hypre_ParCompGridSetSize ( hypre_ParCompGrid *compGrid, HYPRE_Int num_nodes, HYPRE_Int mem_size, HYPRE_Int A_nnz, HYPRE_Int P_nnz, HYPRE_Int full_comp_info )
{
   hypre_ParCompGridNumNodes(compGrid) = num_nodes;
   hypre_ParCompGridMemSize(compGrid) = mem_size;
   HYPRE_Int over_allocation_factor = mem_size;
   if (num_nodes > 0) over_allocation_factor = ceil(mem_size/num_nodes);
   hypre_ParCompGridAMemSize(compGrid) = A_nnz*over_allocation_factor;
   hypre_ParCompGridPMemSize(compGrid) = P_nnz*over_allocation_factor;
   
   hypre_ParCompGridARowPtr(compGrid) = hypre_CTAlloc(HYPRE_Int, mem_size+1, HYPRE_MEMORY_HOST);
   hypre_ParCompGridAColInd(compGrid) = hypre_CTAlloc(HYPRE_Int, hypre_ParCompGridAMemSize(compGrid), HYPRE_MEMORY_HOST);
   if (full_comp_info) hypre_ParCompGridAGlobalColInd(compGrid) = hypre_CTAlloc(HYPRE_Int, hypre_ParCompGridAMemSize(compGrid), HYPRE_MEMORY_HOST);
   hypre_ParCompGridAData(compGrid) = hypre_CTAlloc(HYPRE_Complex, hypre_ParCompGridAMemSize(compGrid), HYPRE_MEMORY_HOST);

   if (full_comp_info)
   {
      hypre_ParCompGridGlobalIndices(compGrid) = hypre_CTAlloc(HYPRE_Int, mem_size, HYPRE_MEMORY_HOST);
   }
   if (full_comp_info > 1)
   {
      hypre_ParCompGridCoarseGlobalIndices(compGrid) = hypre_CTAlloc(HYPRE_Int, mem_size, HYPRE_MEMORY_HOST);
      hypre_ParCompGridCoarseLocalIndices(compGrid) = hypre_CTAlloc(HYPRE_Int, mem_size, HYPRE_MEMORY_HOST);
   }

   hypre_ParCompGridPRowPtr(compGrid) = hypre_CTAlloc(HYPRE_Int, mem_size+1, HYPRE_MEMORY_HOST);
   if (P_nnz)
   {
      hypre_ParCompGridPColInd(compGrid) = hypre_CTAlloc(HYPRE_Int, hypre_ParCompGridPMemSize(compGrid), HYPRE_MEMORY_HOST);
      hypre_ParCompGridPData(compGrid) = hypre_CTAlloc(HYPRE_Complex, hypre_ParCompGridPMemSize(compGrid), HYPRE_MEMORY_HOST);      
   }

   return 0;
}

HYPRE_Int
hypre_ParCompGridResize ( hypre_ParCompGrid *compGrid, HYPRE_Int new_size, HYPRE_Int need_coarse_info, HYPRE_Int type )
{
   // This function reallocates memory to hold a comp grid of size new_size
   // num_nodes and mem_size are set to new_size. Use this when exact size of new comp grid is known.

   // Reallocate num nodes
   if (type == 0)
   {
      // Re allocate to given size
      hypre_ParCompGridGlobalIndices(compGrid) = hypre_TReAlloc(hypre_ParCompGridGlobalIndices(compGrid), HYPRE_Int, new_size, HYPRE_MEMORY_HOST);
      hypre_ParCompGridRealDofMarker(compGrid) = hypre_TReAlloc(hypre_ParCompGridRealDofMarker(compGrid), HYPRE_Int, new_size, HYPRE_MEMORY_HOST);
      hypre_ParCompGridARowPtr(compGrid) = hypre_TReAlloc(hypre_ParCompGridARowPtr(compGrid), HYPRE_Int, new_size+1, HYPRE_MEMORY_HOST);
      if (need_coarse_info)
      {
         hypre_ParCompGridCoarseGlobalIndices(compGrid) = hypre_TReAlloc(hypre_ParCompGridCoarseGlobalIndices(compGrid), HYPRE_Int, new_size, HYPRE_MEMORY_HOST);
         hypre_ParCompGridCoarseLocalIndices(compGrid) = hypre_TReAlloc(hypre_ParCompGridCoarseLocalIndices(compGrid), HYPRE_Int, new_size, HYPRE_MEMORY_HOST);
         hypre_ParCompGridPRowPtr(compGrid) = hypre_TReAlloc(hypre_ParCompGridPRowPtr(compGrid), HYPRE_Int, new_size+1, HYPRE_MEMORY_HOST);
      }
      hypre_ParCompGridMemSize(compGrid) = new_size;  
   }
   // Reallocate A matrix
   else if (type == 1)
   {
      hypre_ParCompGridAColInd(compGrid) = hypre_TReAlloc(hypre_ParCompGridAColInd(compGrid), HYPRE_Int, new_size, HYPRE_MEMORY_HOST);
      hypre_ParCompGridAGlobalColInd(compGrid) = hypre_TReAlloc(hypre_ParCompGridAGlobalColInd(compGrid), HYPRE_Int, new_size, HYPRE_MEMORY_HOST);
      hypre_ParCompGridAData(compGrid) = hypre_TReAlloc(hypre_ParCompGridAData(compGrid), HYPRE_Complex, new_size, HYPRE_MEMORY_HOST);
      hypre_ParCompGridAMemSize(compGrid) = new_size;
   }
   // Reallocate P matrix
   else if (type == 2)
   {
      hypre_ParCompGridPColInd(compGrid) = hypre_TReAlloc(hypre_ParCompGridPColInd(compGrid), HYPRE_Int, new_size, HYPRE_MEMORY_HOST);
      hypre_ParCompGridPData(compGrid) = hypre_TReAlloc(hypre_ParCompGridPData(compGrid), HYPRE_Complex, new_size, HYPRE_MEMORY_HOST);
      hypre_ParCompGridPMemSize(compGrid) = new_size;
   }

   return 0;
}

HYPRE_Int 
hypre_ParCompGridSetupLocalIndices( hypre_ParCompGrid **compGrid, HYPRE_Int *nodes_added_on_level, HYPRE_Int transition_level )
{
   // when nodes are added to a composite grid, global info is copied over, but local indices must be generated appropriately for all added nodes
   // this must be done on each level as info is added to correctly construct subsequent Psi_c grids
   // also done after each ghost layer is added
   HYPRE_Int      level,i,j,k;
   HYPRE_Int      global_index, coarse_global_index, local_index;

   HYPRE_Int myid;
   hypre_MPI_Comm_rank(hypre_MPI_COMM_WORLD, &myid);


   for (level = 0; level < transition_level; level++)
   {
      // If we have added nodes on this level
      if (nodes_added_on_level[level])
      {
         // loop over indices on this level !!! This could be made more efficient! I now loop over all the owned nodes, and most of these don't need any work, yet I'm doing it anyway...
         for (i = 0; i < hypre_ParCompGridNumNodes(compGrid[level]); i++)
         {
            // fix up the local indices for the matrix A row info
            for (j = hypre_ParCompGridARowPtr(compGrid[level])[i]; j < hypre_ParCompGridARowPtr(compGrid[level])[i+1]; j++)
            {
               // initialize local index to -1 (it will be set to another value only if we can find the global index somewhere in this comp grid)
               global_index = hypre_ParCompGridAGlobalColInd(compGrid[level])[j];
               local_index = -global_index-1;
               
               // if global index of j'th row entry is owned by this proc, then local index is calculable
               HYPRE_Int num_owned_blocks = hypre_ParCompGridNumOwnedBlocks(compGrid[level]);
               for (k = 0; k < num_owned_blocks; k++)
               {
                  if (hypre_ParCompGridOwnedBlockStarts(compGrid[level])[k+1] - hypre_ParCompGridOwnedBlockStarts(compGrid[level])[k] > 0)
                  {
                     HYPRE_Int low_global_index = hypre_ParCompGridGlobalIndices(compGrid[level])[ hypre_ParCompGridOwnedBlockStarts(compGrid[level])[k] ];
                     HYPRE_Int high_global_index = hypre_ParCompGridGlobalIndices(compGrid[level])[ hypre_ParCompGridOwnedBlockStarts(compGrid[level])[k+1] - 1 ];
                     
                     if ( global_index >= low_global_index && global_index <= high_global_index )
                     {
                        // set local index for entry in this row of the matrix
                        local_index = global_index - low_global_index + hypre_ParCompGridOwnedBlockStarts(compGrid[level])[k];
                     }
                  }
               }
               // otherwise find local index via binary search
               if (local_index < 0)
               {
                  local_index = hypre_ParCompGridLocalIndexBinarySearch(compGrid[level], global_index, 0);
                  if (local_index == -1) local_index = -global_index-1;
               }
               hypre_ParCompGridAColInd(compGrid[level])[j] = local_index;
            }
         }
      }
      
      // if we are not on the coarsest level
      if (level != transition_level-1)
      {
         if (nodes_added_on_level[level] || nodes_added_on_level[level+1])
         {
            // loop over indices of non-owned nodes on this level
            HYPRE_Int num_owned_nodes = hypre_ParCompGridOwnedBlockStarts(compGrid[level])[hypre_ParCompGridNumOwnedBlocks(compGrid[level])];
            for (i = num_owned_nodes; i < hypre_ParCompGridNumNodes(compGrid[level]); i++)
            {
               // fix up the coarse local indices
               coarse_global_index = hypre_ParCompGridCoarseGlobalIndices(compGrid[level])[i];

               // if this node is repeated on the next coarsest grid, figure out its local index
               hypre_ParCompGridCoarseLocalIndices(compGrid[level])[i] = -1;
               if ( coarse_global_index != -1)
               {
                  // Check whether this node belongs to an owned block on the next level
                  for (j = 0; j < hypre_ParCompGridNumOwnedBlocks(compGrid[level+1]); j++)
                  {
                     if (hypre_ParCompGridOwnedBlockStarts(compGrid[level+1])[j+1] - hypre_ParCompGridOwnedBlockStarts(compGrid[level+1])[j] > 0)
                     {
                        HYPRE_Int low_global_index = hypre_ParCompGridGlobalIndices(compGrid[level+1])[hypre_ParCompGridOwnedBlockStarts(compGrid[level+1])[j]];
                        HYPRE_Int high_global_index = hypre_ParCompGridGlobalIndices(compGrid[level+1])[hypre_ParCompGridOwnedBlockStarts(compGrid[level+1])[j+1]-1];
                        if (coarse_global_index >= low_global_index && coarse_global_index <= high_global_index)
                        {
                           hypre_ParCompGridCoarseLocalIndices(compGrid[level])[i] = coarse_global_index - low_global_index + hypre_ParCompGridOwnedBlockStarts(compGrid[level+1])[j];
                           break;
                        }
                     }
                  }

                  if (hypre_ParCompGridCoarseLocalIndices(compGrid[level])[i] < 0)
                     hypre_ParCompGridCoarseLocalIndices(compGrid[level])[i] = hypre_ParCompGridLocalIndexBinarySearch(compGrid[level+1], coarse_global_index, 0);
               }
            }
         }
      }
   }

   return 0;
}

HYPRE_Int hypre_ParCompGridSetupLocalIndicesP( hypre_ParCompGrid **compGrid, HYPRE_Int num_levels, HYPRE_Int transition_level )
{
   HYPRE_Int                  i,j,level,global_index;

   for (level = 0; level < transition_level-1; level++)
   {
      HYPRE_Int num_owned_blocks = hypre_ParCompGridNumOwnedBlocks(compGrid[level+1]);

      // Setup all local indices for all nodes (note that PColInd currently stores global indices)
      for (i = 0; i < hypre_ParCompGridPRowPtr(compGrid[level])[ hypre_ParCompGridNumNodes(compGrid[level]) ]; i++)
      {
         global_index = hypre_ParCompGridPColInd(compGrid[level])[i];
         hypre_ParCompGridPColInd(compGrid[level])[i] = -global_index - 1;
         // If global index is owned, simply calculate
         for (j = 0; j < num_owned_blocks; j++)
         {
            if (hypre_ParCompGridOwnedBlockStarts(compGrid[level+1])[j+1] - hypre_ParCompGridOwnedBlockStarts(compGrid[level+1])[j] > 0)
            {
               HYPRE_Int low_global_index = hypre_ParCompGridGlobalIndices(compGrid[level+1])[ hypre_ParCompGridOwnedBlockStarts(compGrid[level+1])[j] ];
               HYPRE_Int high_global_index = hypre_ParCompGridGlobalIndices(compGrid[level+1])[ hypre_ParCompGridOwnedBlockStarts(compGrid[level+1])[j+1] - 1 ];
               if ( global_index >= low_global_index && global_index <= high_global_index )
               {
                  hypre_ParCompGridPColInd(compGrid[level])[i] = global_index - low_global_index + hypre_ParCompGridOwnedBlockStarts(compGrid[level+1])[j];
               }
            }
         }
         // Otherwise, binary search
         if (hypre_ParCompGridPColInd(compGrid[level])[i] < 0) hypre_ParCompGridPColInd(compGrid[level])[i] = hypre_ParCompGridLocalIndexBinarySearch(compGrid[level+1], global_index, 0);
         if (hypre_ParCompGridPColInd(compGrid[level])[i] < 0) hypre_ParCompGridPColInd(compGrid[level])[i] = -global_index - 1;
      }
   }

   return 0;
}

HYPRE_Int hypre_ParCompGridLocalIndexBinarySearch( hypre_ParCompGrid *compGrid, HYPRE_Int global_index, HYPRE_Int allow_failed_search )
{
   HYPRE_Int      num_owned_nodes = hypre_ParCompGridOwnedBlockStarts(compGrid)[hypre_ParCompGridNumOwnedBlocks(compGrid)];
   HYPRE_Int      left = num_owned_nodes;
   HYPRE_Int      right = hypre_ParCompGridNumNodes(compGrid) - 1;
   HYPRE_Int      index;

   HYPRE_Int      myid;
   hypre_MPI_Comm_rank(hypre_MPI_COMM_WORLD, &myid );

   while (left <= right)
   {
      index = (left + right) / 2;
      if (hypre_ParCompGridGlobalIndices(compGrid)[index] < global_index) left = index + 1;
      else if (hypre_ParCompGridGlobalIndices(compGrid)[index] > global_index) right = index - 1;
      else return index;
   }

   // If binary search fails to find an exact match, return the index of the first element greater than global_index or -1
   if (allow_failed_search) return left;
   else return -1;
}

HYPRE_Int
hypre_ParCompGridDebugPrint ( hypre_ParCompGrid *compGrid, const char* filename )
{
   HYPRE_Int      myid;
   hypre_MPI_Comm_rank(hypre_MPI_COMM_WORLD, &myid );

   // Get composite grid information
   HYPRE_Int       num_nodes = hypre_ParCompGridNumNodes(compGrid);
   HYPRE_Int       num_owned_blocks = hypre_ParCompGridNumOwnedBlocks(compGrid);
   HYPRE_Int       num_owned_nodes = hypre_ParCompGridOwnedBlockStarts(compGrid)[hypre_ParCompGridNumOwnedBlocks(compGrid)];
   HYPRE_Int       mem_size = hypre_ParCompGridMemSize(compGrid);
   HYPRE_Int       A_mem_size = hypre_ParCompGridAMemSize(compGrid);
   HYPRE_Int       P_mem_size = hypre_ParCompGridPMemSize(compGrid);

   HYPRE_Int        *global_indices = hypre_ParCompGridGlobalIndices(compGrid);
   HYPRE_Int        *coarse_global_indices = hypre_ParCompGridCoarseGlobalIndices(compGrid);
   HYPRE_Int        *coarse_local_indices = hypre_ParCompGridCoarseLocalIndices(compGrid);

   HYPRE_Int *A_rowptr = hypre_ParCompGridARowPtr(compGrid);
   HYPRE_Int *A_colind = hypre_ParCompGridAColInd(compGrid);
   HYPRE_Int *A_global_colind = hypre_ParCompGridAGlobalColInd(compGrid);
   HYPRE_Complex *A_data = hypre_ParCompGridAData(compGrid);
   HYPRE_Int *P_rowptr = hypre_ParCompGridPRowPtr(compGrid);
   HYPRE_Int *P_colind = hypre_ParCompGridPColInd(compGrid);
   HYPRE_Complex *P_data = hypre_ParCompGridPData(compGrid);

   HYPRE_Int         i;

   // Measure number of ghost nodes
   HYPRE_Int num_real = 0;
   for (i = 0; i < num_nodes; i++) if (A_rowptr[i+1] - A_rowptr[i] > 0) num_real++;

   // Print info to given filename   
   FILE             *file;
   file = fopen(filename,"w");
   hypre_fprintf(file, "Num nodes: %d\nMem size: %d\nA Mem size: %d\nP Mem size: %d\nNum owned nodes: %d\nNum ghost dofs: %d\nNum real dofs: %d\n", 
      num_nodes, mem_size, A_mem_size, P_mem_size, num_owned_nodes, num_nodes - num_real, num_real);
   hypre_fprintf(file, "Num owned blocks = %d\n", num_owned_blocks);
   hypre_fprintf(file, "owned_block_starts = ");
   for (i = 0; i < num_owned_blocks+1; i++) hypre_fprintf(file, "%d ", hypre_ParCompGridOwnedBlockStarts(compGrid)[i]);
   hypre_fprintf(file,"\n");
   // hypre_fprintf(file, "u:\n");
   // for (i = 0; i < num_nodes; i++)
   // {
   //    hypre_fprintf(file, "%.10f ", u[i]);
   // }
   // hypre_fprintf(file, "\n");
   // hypre_fprintf(file, "f:\n");
   // for (i = 0; i < num_nodes; i++)
   // {
   //    hypre_fprintf(file, "%.10f ", f[i]);
   // }
   // hypre_fprintf(file, "\n");
   if (global_indices)
   {
      hypre_fprintf(file, "global_indices:\n");
      for (i = 0; i < num_nodes; i++)
      {
         hypre_fprintf(file, "%d ", global_indices[i]);
      }
   }
   if (coarse_global_indices)
   {
      hypre_fprintf(file, "\n");
      hypre_fprintf(file, "coarse_global_indices:\n");
      for (i = 0; i < num_nodes; i++)
      {
         hypre_fprintf(file, "%d ", coarse_global_indices[i]);
      }
      hypre_fprintf(file, "\n");
      hypre_fprintf(file, "coarse_local_indices:\n");
      for (i = 0; i < num_nodes; i++)
      {
         hypre_fprintf(file, "%d ", coarse_local_indices[i]);
      }
      hypre_fprintf(file, "\n");
   }
   hypre_fprintf(file, "\n");

   if (A_rowptr)
   {
      hypre_fprintf(file, "\nA row pointer:\n");
      for (i = 0; i < num_nodes+1; i++) hypre_fprintf(file, "%d ", A_rowptr[i]);
      hypre_fprintf(file,"\n\n");
      hypre_fprintf(file, "A colind:\n");
      for (i = 0; i < A_rowptr[num_nodes]; i++) hypre_fprintf(file, "%d ", A_colind[i]);
      hypre_fprintf(file,"\n\n");
      if (A_global_colind)
      {
         hypre_fprintf(file, "A global colind:\n");
         for (i = 0; i < A_rowptr[num_nodes]; i++) hypre_fprintf(file, "%d ", A_global_colind[i]);
         hypre_fprintf(file,"\n\n");
      }
      hypre_fprintf(file, "A data:\n");
      for (i = 0; i < A_rowptr[num_nodes]; i++) hypre_fprintf(file, "%f ", A_data[i]);
   }
   if (P_rowptr)
   {
      hypre_fprintf(file,"\n\n");
      hypre_fprintf(file, "P row pointer:\n");
      for (i = 0; i < num_nodes+1; i++) hypre_fprintf(file, "%d ", P_rowptr[i]);
      hypre_fprintf(file,"\n\n");
      hypre_fprintf(file, "P colind:\n");
      for (i = 0; i < P_rowptr[num_nodes]; i++) hypre_fprintf(file, "%d ", P_colind[i]);
      hypre_fprintf(file,"\n\n");
      hypre_fprintf(file, "P data:\n");
      for (i = 0; i < P_rowptr[num_nodes]; i++) hypre_fprintf(file, "%f ", P_data[i]);
   }

   fclose(file);

   return 0;

}

HYPRE_Int 
hypre_ParCompGridDumpSorted( hypre_ParCompGrid *compGrid, const char* filename)
{
   // Check whether we have anything to dump
   if (!hypre_ParCompGridGlobalIndices(compGrid))
   {
      printf("Trying to dump comp grid, but no global indices\n");
      return 0;
   }

   // Get composite grid information
   HYPRE_Int        *global_indices = hypre_ParCompGridGlobalIndices(compGrid);

   // Get the position where the owned nodes should go in order to output arrays sorted by global index
   HYPRE_Int insert_owned_position;
   HYPRE_Int num_owned_nodes = hypre_ParCompGridOwnedBlockStarts(compGrid)[hypre_ParCompGridNumOwnedBlocks(compGrid)];
   if (num_owned_nodes)
   {
      HYPRE_Int first_owned = hypre_ParCompGridGlobalIndices(compGrid)[0];
      HYPRE_Int last_owned = hypre_ParCompGridGlobalIndices(compGrid)[ num_owned_nodes - 1 ];
      HYPRE_Int first_nonowned = hypre_ParCompGridGlobalIndices(compGrid)[ num_owned_nodes ];
      HYPRE_Int last_nonowned = hypre_ParCompGridGlobalIndices(compGrid)[ hypre_ParCompGridNumNodes(compGrid) - 1 ];

      // Find where to insert owned nodes in the list of all comp grid nodes (such that they are ordered according to global index)
      if (last_owned < first_nonowned) insert_owned_position = num_owned_nodes;
      else if (first_owned > last_nonowned) insert_owned_position = hypre_ParCompGridNumNodes(compGrid);
      else
      {
         // Binary search to find where to insert
         insert_owned_position = hypre_ParCompGridLocalIndexBinarySearch(compGrid, first_owned, 1);
      }
   }
   else insert_owned_position = 0;

   // Print info to given filename   
   FILE             *file;
   file = fopen(filename,"w");
   HYPRE_Int i;

   // Global indices
   for (i = num_owned_nodes; i < insert_owned_position; i++)
   {
      hypre_fprintf(file, "%d ", global_indices[i]);
   }
   for (i = 0; i < num_owned_nodes; i++)
   {
      hypre_fprintf(file, "%d ", global_indices[i]);
   }
   for (i = insert_owned_position; i < hypre_ParCompGridNumNodes(compGrid); i++)
   {
      hypre_fprintf(file, "%d ", global_indices[i]);
   }
   hypre_fprintf(file, "\n");

   fclose(file);

   return 0;
}

HYPRE_Int 
hypre_ParCompGridGlobalIndicesDump( hypre_ParCompGrid *compGrid, const char* filename)
{
   FILE             *file;
   file = fopen(filename,"w");
   HYPRE_Int i;

   // Global indices
   for (i = 0; i < hypre_ParCompGridNumNodes(compGrid); i++)
   {
      hypre_fprintf(file, "%d\n", hypre_ParCompGridGlobalIndices(compGrid)[i]);
   }

   fclose(file);

   return 0;
}

HYPRE_Int 
hypre_ParCompGridCoarseGlobalIndicesDump( hypre_ParCompGrid *compGrid, const char* filename)
{
      FILE             *file;
      file = fopen(filename,"w");
      HYPRE_Int i;

   if (hypre_ParCompGridCoarseGlobalIndices(compGrid))
   {
      // Global indices
      for (i = 0; i < hypre_ParCompGridNumNodes(compGrid); i++)
      {
         hypre_fprintf(file, "%d\n", hypre_ParCompGridCoarseGlobalIndices(compGrid)[i]);
      }

      fclose(file);
   }

   return 0;
}

HYPRE_Int
hypre_ParCompGridMatlabAMatrixDump( hypre_ParCompGrid *compGrid, const char* filename)
{
   // Get composite grid information
   HYPRE_Int       num_nodes = hypre_ParCompGridNumNodes(compGrid);

   // Print info to given filename   
   FILE             *file;
   file = fopen(filename,"w");
   HYPRE_Int i,j;

   if (hypre_ParCompGridARowPtr(compGrid))
   {
      for (i = 0; i < num_nodes; i++)
      {
         for (j = hypre_ParCompGridARowPtr(compGrid)[i]; j < hypre_ParCompGridARowPtr(compGrid)[i+1]; j++)
         {
            hypre_fprintf(file, "%d ", i);
            hypre_fprintf(file, "%d ", hypre_ParCompGridAColInd(compGrid)[j]);
            hypre_fprintf(file, "%e\n", hypre_ParCompGridAData(compGrid)[j]);
         }
      }
   }

   fclose(file);

   return 0;
}

HYPRE_Int
hypre_ParCompGridMatlabPMatrixDump( hypre_ParCompGrid *compGrid, const char* filename)
{
   // Get composite grid information
   HYPRE_Int       num_nodes = hypre_ParCompGridNumNodes(compGrid);

   // Print info to given filename   
   FILE             *file;
   file = fopen(filename,"w");
   HYPRE_Int i,j;

   if (hypre_ParCompGridPRowPtr(compGrid))
   {
      for (i = 0; i < num_nodes; i++)
      {
         for (j = hypre_ParCompGridPRowPtr(compGrid)[i]; j < hypre_ParCompGridPRowPtr(compGrid)[i+1]; j++)
         {
            hypre_fprintf(file, "%d ", i);
            hypre_fprintf(file, "%d ", hypre_ParCompGridPColInd(compGrid)[j]);
            hypre_fprintf(file, "%e\n", hypre_ParCompGridPData(compGrid)[j]);
         }
      }
   }

   fclose(file);

   return 0;
}

hypre_ParCompGridCommPkg*
hypre_ParCompGridCommPkgCreate()
{
   hypre_ParCompGridCommPkg   *compGridCommPkg;

   compGridCommPkg = hypre_CTAlloc(hypre_ParCompGridCommPkg, 1, HYPRE_MEMORY_HOST);

   hypre_ParCompGridCommPkgNumLevels(compGridCommPkg) = 0;
   hypre_ParCompGridCommPkgTransitionLevel(compGridCommPkg) = -1;
   hypre_ParCompGridCommPkgTransitionResRecvSizes(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgTransitionResRecvDisps(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgAggLocalComms(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgAggGlobalComms(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgNumSendProcs(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgNumRecvProcs(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgNumSendPartitions(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgSendProcs(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgRecvProcs(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgSendPartitions(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgSendProcPartitions(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgSendPartitionRanks(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgSendMapStarts(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgSendMapElmts(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgRecvMapStarts(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgRecvMapElmts(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgGhostMarker(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgSendBufferSize(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgRecvBufferSize(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgNumSendNodes(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgNumRecvNodes(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgSendFlag(compGridCommPkg) = NULL;
   hypre_ParCompGridCommPkgRecvMap(compGridCommPkg) = NULL;

   return compGridCommPkg;
}

HYPRE_Int
hypre_ParCompGridCommPkgDestroy( hypre_ParCompGridCommPkg *compGridCommPkg )
{
   HYPRE_Int         i, j, k;

   if ( hypre_ParCompGridCommPkgTransitionResRecvSizes(compGridCommPkg) )
   {
      hypre_TFree(hypre_ParCompGridCommPkgTransitionResRecvSizes(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgAggLocalComms(compGridCommPkg) )
   {
      hypre_TFree(hypre_ParCompGridCommPkgAggLocalComms(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgAggGlobalComms(compGridCommPkg) )
   {
      hypre_TFree(hypre_ParCompGridCommPkgAggGlobalComms(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgTransitionResRecvDisps(compGridCommPkg) )
   {
      hypre_TFree(hypre_ParCompGridCommPkgTransitionResRecvDisps(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgSendProcs(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         hypre_TFree(hypre_ParCompGridCommPkgSendProcs(compGridCommPkg)[i], HYPRE_MEMORY_HOST);
      }
      hypre_TFree(hypre_ParCompGridCommPkgSendProcs(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgRecvProcs(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         hypre_TFree(hypre_ParCompGridCommPkgRecvProcs(compGridCommPkg)[i], HYPRE_MEMORY_HOST);
      }
      hypre_TFree(hypre_ParCompGridCommPkgRecvProcs(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgSendMapStarts(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         if (hypre_ParCompGridCommPkgSendMapStarts(compGridCommPkg)[i])
            hypre_TFree(hypre_ParCompGridCommPkgSendMapStarts(compGridCommPkg)[i], HYPRE_MEMORY_SHARED);
      }
      hypre_TFree(hypre_ParCompGridCommPkgSendMapStarts(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgSendMapElmts(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         if (hypre_ParCompGridCommPkgSendMapElmts(compGridCommPkg)[i])
            hypre_TFree(hypre_ParCompGridCommPkgSendMapElmts(compGridCommPkg)[i], HYPRE_MEMORY_SHARED);
      }
      hypre_TFree(hypre_ParCompGridCommPkgSendMapElmts(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgRecvMapStarts(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         if (hypre_ParCompGridCommPkgRecvMapStarts(compGridCommPkg)[i])
            hypre_TFree(hypre_ParCompGridCommPkgRecvMapStarts(compGridCommPkg)[i], HYPRE_MEMORY_SHARED);
      }
      hypre_TFree(hypre_ParCompGridCommPkgRecvMapStarts(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgRecvMapElmts(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         if (hypre_ParCompGridCommPkgRecvMapElmts(compGridCommPkg)[i])
            hypre_TFree(hypre_ParCompGridCommPkgRecvMapElmts(compGridCommPkg)[i], HYPRE_MEMORY_SHARED);
      }
      hypre_TFree(hypre_ParCompGridCommPkgRecvMapElmts(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgGhostMarker(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         hypre_TFree(hypre_ParCompGridCommPkgGhostMarker(compGridCommPkg)[i], HYPRE_MEMORY_HOST);
      }
      hypre_TFree(hypre_ParCompGridCommPkgGhostMarker(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgSendPartitions(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         hypre_TFree(hypre_ParCompGridCommPkgSendPartitions(compGridCommPkg)[i], HYPRE_MEMORY_HOST);
      }
      hypre_TFree(hypre_ParCompGridCommPkgSendPartitions(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgSendProcPartitions(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         hypre_TFree(hypre_ParCompGridCommPkgSendProcPartitions(compGridCommPkg)[i], HYPRE_MEMORY_HOST);
      }
      hypre_TFree(hypre_ParCompGridCommPkgSendProcPartitions(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgSendPartitionRanks(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         if (hypre_ParCompGridCommPkgSendPartitionRanks(compGridCommPkg)[i])
         {
            for (j = 0; j < hypre_ParCompGridCommPkgNumSendPartitions(compGridCommPkg)[i]; j++)
            {
               hypre_TFree(hypre_ParCompGridCommPkgSendPartitionRanks(compGridCommPkg)[i][j], HYPRE_MEMORY_HOST);
            }
            hypre_TFree(hypre_ParCompGridCommPkgSendPartitionRanks(compGridCommPkg)[i], HYPRE_MEMORY_HOST);
         }
      }
      hypre_TFree(hypre_ParCompGridCommPkgSendPartitionRanks(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgSendBufferSize(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         hypre_TFree(hypre_ParCompGridCommPkgSendBufferSize(compGridCommPkg)[i], HYPRE_MEMORY_HOST);
      }
      hypre_TFree(hypre_ParCompGridCommPkgSendBufferSize(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgRecvBufferSize(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         hypre_TFree(hypre_ParCompGridCommPkgRecvBufferSize(compGridCommPkg)[i], HYPRE_MEMORY_HOST);
      }
      hypre_TFree(hypre_ParCompGridCommPkgRecvBufferSize(compGridCommPkg), HYPRE_MEMORY_HOST);
   }

   if ( hypre_ParCompGridCommPkgSendFlag(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         for (j = 0; j < hypre_ParCompGridCommPkgNumSendPartitions(compGridCommPkg)[i]; j++)
         {
            for (k = 0; k < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); k++)
            {
               if ( hypre_ParCompGridCommPkgSendFlag(compGridCommPkg)[i][j][k] ) hypre_TFree( hypre_ParCompGridCommPkgSendFlag(compGridCommPkg)[i][j][k], HYPRE_MEMORY_HOST );
            }
            hypre_TFree( hypre_ParCompGridCommPkgSendFlag(compGridCommPkg)[i][j], HYPRE_MEMORY_HOST );
         }
         hypre_TFree( hypre_ParCompGridCommPkgSendFlag(compGridCommPkg)[i], HYPRE_MEMORY_HOST );
      }
      hypre_TFree( hypre_ParCompGridCommPkgSendFlag(compGridCommPkg), HYPRE_MEMORY_HOST );
   }

   if ( hypre_ParCompGridCommPkgRecvMap(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         for (j = 0; j < hypre_ParCompGridCommPkgNumRecvProcs(compGridCommPkg)[i]; j++)
         {
            for (k = 0; k < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); k++)
            {
               if ( hypre_ParCompGridCommPkgRecvMap(compGridCommPkg)[i][j][k] ) hypre_TFree( hypre_ParCompGridCommPkgRecvMap(compGridCommPkg)[i][j][k], HYPRE_MEMORY_SHARED );
            }
            hypre_TFree( hypre_ParCompGridCommPkgRecvMap(compGridCommPkg)[i][j], HYPRE_MEMORY_HOST );
         }
         hypre_TFree( hypre_ParCompGridCommPkgRecvMap(compGridCommPkg)[i], HYPRE_MEMORY_HOST );
      }
      hypre_TFree( hypre_ParCompGridCommPkgRecvMap(compGridCommPkg), HYPRE_MEMORY_HOST );
   }

   if ( hypre_ParCompGridCommPkgNumSendNodes(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         for (j = 0; j < hypre_ParCompGridCommPkgNumSendPartitions(compGridCommPkg)[i]; j++)
         {
            hypre_TFree( hypre_ParCompGridCommPkgNumSendNodes(compGridCommPkg)[i][j], HYPRE_MEMORY_HOST );
         }
         hypre_TFree( hypre_ParCompGridCommPkgNumSendNodes(compGridCommPkg)[i], HYPRE_MEMORY_HOST );
      }
      hypre_TFree( hypre_ParCompGridCommPkgNumSendNodes(compGridCommPkg), HYPRE_MEMORY_HOST );
   }

   if ( hypre_ParCompGridCommPkgNumRecvNodes(compGridCommPkg) )
   {
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         for (j = 0; j < hypre_ParCompGridCommPkgNumRecvProcs(compGridCommPkg)[i]; j++)
         {
            hypre_TFree( hypre_ParCompGridCommPkgNumRecvNodes(compGridCommPkg)[i][j], HYPRE_MEMORY_HOST );
         }
         hypre_TFree( hypre_ParCompGridCommPkgNumRecvNodes(compGridCommPkg)[i], HYPRE_MEMORY_HOST );
      }
      hypre_TFree( hypre_ParCompGridCommPkgNumRecvNodes(compGridCommPkg), HYPRE_MEMORY_HOST );
   }

   if ( hypre_ParCompGridCommPkgNumSendProcs(compGridCommPkg) )
   {
      hypre_TFree( hypre_ParCompGridCommPkgNumSendProcs(compGridCommPkg), HYPRE_MEMORY_HOST );
   }

   if ( hypre_ParCompGridCommPkgNumRecvProcs(compGridCommPkg) )
   {
      hypre_TFree( hypre_ParCompGridCommPkgNumRecvProcs(compGridCommPkg), HYPRE_MEMORY_HOST );
   }
   
   if  ( hypre_ParCompGridCommPkgNumSendPartitions(compGridCommPkg) )
   {
      hypre_TFree( hypre_ParCompGridCommPkgNumSendPartitions(compGridCommPkg), HYPRE_MEMORY_HOST );
   }

   hypre_TFree(compGridCommPkg, HYPRE_MEMORY_HOST);

   return 0;
}


hypre_ParCompGridCommPkg*
hypre_ParCompGridCommPkgCopy( hypre_ParCompGridCommPkg *compGridCommPkg )
{
   HYPRE_Int   num_procs;
   hypre_MPI_Comm_size(hypre_MPI_COMM_WORLD, &num_procs);

   hypre_ParCompGridCommPkg *copy_compGridCommPkg = hypre_ParCompGridCommPkgCreate();

   HYPRE_Int num_levels = hypre_ParCompGridCommPkgNumLevels(compGridCommPkg);
   HYPRE_Int transition_level = hypre_ParCompGridCommPkgTransitionLevel(compGridCommPkg);
   hypre_ParCompGridCommPkgNumLevels(copy_compGridCommPkg) = num_levels;
   hypre_ParCompGridCommPkgTransitionLevel(copy_compGridCommPkg) = transition_level;

   HYPRE_Int         i, j, k, l;

   if ( hypre_ParCompGridCommPkgTransitionResRecvSizes(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgTransitionResRecvSizes(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int, num_procs, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_procs; i++) 
         hypre_ParCompGridCommPkgTransitionResRecvSizes(copy_compGridCommPkg)[i] = hypre_ParCompGridCommPkgTransitionResRecvSizes(compGridCommPkg)[i];
   }

   if ( hypre_ParCompGridCommPkgAggLocalComms(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgAggLocalComms(copy_compGridCommPkg) = hypre_CTAlloc(MPI_Comm, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
         hypre_ParCompGridCommPkgAggLocalComms(copy_compGridCommPkg)[i] = hypre_ParCompGridCommPkgAggLocalComms(compGridCommPkg)[i];
   }

   if ( hypre_ParCompGridCommPkgAggGlobalComms(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgAggGlobalComms(copy_compGridCommPkg) = hypre_CTAlloc(MPI_Comm, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
         hypre_ParCompGridCommPkgAggGlobalComms(copy_compGridCommPkg)[i] = hypre_ParCompGridCommPkgAggGlobalComms(compGridCommPkg)[i];
   }

   if ( hypre_ParCompGridCommPkgTransitionResRecvDisps(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgTransitionResRecvDisps(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int, num_procs, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_procs; i++)
         hypre_ParCompGridCommPkgTransitionResRecvDisps(copy_compGridCommPkg)[i] = hypre_ParCompGridCommPkgTransitionResRecvDisps(compGridCommPkg)[i];
   }

   if  ( hypre_ParCompGridCommPkgNumSendProcs(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgNumSendProcs(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
         hypre_ParCompGridCommPkgNumSendProcs(copy_compGridCommPkg)[i] = hypre_ParCompGridCommPkgNumSendProcs(compGridCommPkg)[i];
   }

   if  ( hypre_ParCompGridCommPkgNumRecvProcs(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgNumRecvProcs(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
         hypre_ParCompGridCommPkgNumRecvProcs(copy_compGridCommPkg)[i] = hypre_ParCompGridCommPkgNumRecvProcs(compGridCommPkg)[i];
   }
   
   if  ( hypre_ParCompGridCommPkgNumSendPartitions(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgNumSendPartitions(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
         hypre_ParCompGridCommPkgNumSendPartitions(copy_compGridCommPkg)[i] = hypre_ParCompGridCommPkgNumSendPartitions(compGridCommPkg)[i];
   }

   if ( hypre_ParCompGridCommPkgSendProcs(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgSendProcs(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int*, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
      {
         HYPRE_Int num_comm_procs = hypre_ParCompGridCommPkgNumSendProcs(compGridCommPkg)[i];
         hypre_ParCompGridCommPkgSendProcs(copy_compGridCommPkg)[i] = hypre_CTAlloc(HYPRE_Int, num_comm_procs, HYPRE_MEMORY_HOST);
         for (j = 0; j < num_comm_procs; j++)
            hypre_ParCompGridCommPkgSendProcs(copy_compGridCommPkg)[i][j] = hypre_ParCompGridCommPkgSendProcs(compGridCommPkg)[i][j];
      }
   }

   if ( hypre_ParCompGridCommPkgRecvProcs(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgRecvProcs(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int*, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
      {
         HYPRE_Int num_comm_procs = hypre_ParCompGridCommPkgNumRecvProcs(compGridCommPkg)[i];
         hypre_ParCompGridCommPkgRecvProcs(copy_compGridCommPkg)[i] = hypre_CTAlloc(HYPRE_Int, num_comm_procs, HYPRE_MEMORY_HOST);
         for (j = 0; j < num_comm_procs; j++)
            hypre_ParCompGridCommPkgRecvProcs(copy_compGridCommPkg)[i][j] = hypre_ParCompGridCommPkgRecvProcs(compGridCommPkg)[i][j];
      }
   }

   if ( hypre_ParCompGridCommPkgSendPartitions(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgSendPartitions(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int*, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
      {
         HYPRE_Int num_comm_partitions = hypre_ParCompGridCommPkgNumSendPartitions(compGridCommPkg)[i];
         hypre_ParCompGridCommPkgSendPartitions(copy_compGridCommPkg)[i] = hypre_CTAlloc(HYPRE_Int, num_comm_partitions, HYPRE_MEMORY_HOST);
         for (j = 0; j < num_comm_partitions; j++)
            hypre_ParCompGridCommPkgSendPartitions(copy_compGridCommPkg)[i][j] = hypre_ParCompGridCommPkgSendPartitions(compGridCommPkg)[i][j];
      }
   }

   if ( hypre_ParCompGridCommPkgSendMapStarts(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgSendMapStarts(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int*, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
      {
         if (hypre_ParCompGridCommPkgSendMapStarts(compGridCommPkg)[i])
         {
            HYPRE_Int num_comm_partitions = hypre_ParCompGridCommPkgNumSendPartitions(compGridCommPkg)[i];
            hypre_ParCompGridCommPkgSendMapStarts(copy_compGridCommPkg)[i] = hypre_CTAlloc(HYPRE_Int, num_comm_partitions+1, HYPRE_MEMORY_HOST);
            for (j = 0; j < num_comm_partitions+1; j++)
               hypre_ParCompGridCommPkgSendMapStarts(copy_compGridCommPkg)[i][j] = hypre_ParCompGridCommPkgSendMapStarts(compGridCommPkg)[i][j];
         }
      }
   }

   if ( hypre_ParCompGridCommPkgSendMapElmts(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgSendMapElmts(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int*, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
      {
         if (hypre_ParCompGridCommPkgSendMapElmts(compGridCommPkg)[i])
         {
            HYPRE_Int num_elmts = hypre_ParCompGridCommPkgSendMapStarts(compGridCommPkg)[i][ hypre_ParCompGridCommPkgNumSendPartitions(compGridCommPkg)[i] ];
            hypre_ParCompGridCommPkgSendMapElmts(copy_compGridCommPkg)[i] = hypre_CTAlloc(HYPRE_Int, num_elmts, HYPRE_MEMORY_HOST);
            for (j = 0; j < num_elmts; j++)
               hypre_ParCompGridCommPkgSendMapElmts(copy_compGridCommPkg)[i][j] = hypre_ParCompGridCommPkgSendMapElmts(compGridCommPkg)[i][j];
         }
      }
   }

   if ( hypre_ParCompGridCommPkgGhostMarker(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgGhostMarker(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int*, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
      {
         if (hypre_ParCompGridCommPkgGhostMarker(compGridCommPkg)[i])
         {
            HYPRE_Int num_elmts = hypre_ParCompGridCommPkgSendMapStarts(compGridCommPkg)[i][ hypre_ParCompGridCommPkgNumSendPartitions(compGridCommPkg)[i] ];
            hypre_ParCompGridCommPkgGhostMarker(copy_compGridCommPkg)[i] = hypre_CTAlloc(HYPRE_Int, num_elmts, HYPRE_MEMORY_HOST);
            for (j = 0; j < num_elmts; j++)
               hypre_ParCompGridCommPkgGhostMarker(copy_compGridCommPkg)[i][j] = hypre_ParCompGridCommPkgGhostMarker(compGridCommPkg)[i][j];
         }
      }
   }

   if ( hypre_ParCompGridCommPkgSendProcPartitions(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgSendProcPartitions(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int*, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
      {
         HYPRE_Int num_comm_procs = hypre_ParCompGridCommPkgNumSendProcs(compGridCommPkg)[i];
         hypre_ParCompGridCommPkgSendProcPartitions(copy_compGridCommPkg)[i] = hypre_CTAlloc(HYPRE_Int, num_comm_procs, HYPRE_MEMORY_HOST);
         for (j = 0; j < num_comm_procs; j++)
            hypre_ParCompGridCommPkgSendProcPartitions(copy_compGridCommPkg)[i][j] = hypre_ParCompGridCommPkgSendProcPartitions(compGridCommPkg)[i][j];
      }
   }

   if ( hypre_ParCompGridCommPkgSendPartitionRanks(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgSendPartitionRanks(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int**, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < hypre_ParCompGridCommPkgNumLevels(compGridCommPkg); i++)
      {
         if (hypre_ParCompGridCommPkgSendPartitionRanks(compGridCommPkg)[i])
         {
            hypre_ParCompGridCommPkgSendPartitionRanks(copy_compGridCommPkg)[i] = hypre_CTAlloc(HYPRE_Int*, hypre_ParCompGridCommPkgNumSendPartitions(compGridCommPkg)[i], HYPRE_MEMORY_HOST);
            for (j = 0; j < hypre_ParCompGridCommPkgNumSendPartitions(compGridCommPkg)[i]; j++)
            {
               hypre_ParCompGridCommPkgSendPartitionRanks(copy_compGridCommPkg)[i][j] = hypre_CTAlloc(HYPRE_Int, hypre_ParCompGridCommPkgSendPartitionRanks(compGridCommPkg)[i][j][0]+1, HYPRE_MEMORY_HOST);
               for (k = 0; k < hypre_ParCompGridCommPkgSendPartitionRanks(compGridCommPkg)[i][j][0]+1; k++)
               {
                  hypre_ParCompGridCommPkgSendPartitionRanks(copy_compGridCommPkg)[i][j][k] = hypre_ParCompGridCommPkgSendPartitionRanks(compGridCommPkg)[i][j][k];
               }
            }
         }
      }
   }

   if ( hypre_ParCompGridCommPkgSendBufferSize(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgSendBufferSize(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int*, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
      {
         HYPRE_Int num_comm_procs = hypre_ParCompGridCommPkgNumSendProcs(compGridCommPkg)[i];
         hypre_ParCompGridCommPkgSendBufferSize(copy_compGridCommPkg)[i] = hypre_CTAlloc(HYPRE_Int, num_comm_procs, HYPRE_MEMORY_HOST);
         for (j = 0; j < num_comm_procs; j++)
            hypre_ParCompGridCommPkgSendBufferSize(copy_compGridCommPkg)[i][j] = hypre_ParCompGridCommPkgSendBufferSize(compGridCommPkg)[i][j];
      }
   }

   if ( hypre_ParCompGridCommPkgRecvBufferSize(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgRecvBufferSize(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int*, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
      {
         HYPRE_Int num_comm_procs = hypre_ParCompGridCommPkgNumRecvProcs(compGridCommPkg)[i];
         hypre_ParCompGridCommPkgRecvBufferSize(copy_compGridCommPkg)[i] = hypre_CTAlloc(HYPRE_Int, num_comm_procs, HYPRE_MEMORY_HOST);
         for (j = 0; j < num_comm_procs; j++)
            hypre_ParCompGridCommPkgRecvBufferSize(copy_compGridCommPkg)[i][j] = hypre_ParCompGridCommPkgRecvBufferSize(compGridCommPkg)[i][j];
      }
   }

   if ( hypre_ParCompGridCommPkgNumSendNodes(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgNumSendNodes(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int**, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
      {
         HYPRE_Int num_comm_procs = hypre_ParCompGridCommPkgNumSendProcs(compGridCommPkg)[i];
         hypre_ParCompGridCommPkgNumSendNodes(copy_compGridCommPkg)[i] = hypre_CTAlloc(HYPRE_Int*, num_comm_procs, HYPRE_MEMORY_HOST);
         for (j = 0; j < num_comm_procs; j++)
         {
            hypre_ParCompGridCommPkgNumSendNodes(copy_compGridCommPkg)[i][j] = hypre_CTAlloc(HYPRE_Int, num_levels, HYPRE_MEMORY_HOST);
            for (k = 0; k < num_levels; k++)
            {
               hypre_ParCompGridCommPkgNumSendNodes(copy_compGridCommPkg)[i][j][k] = hypre_ParCompGridCommPkgNumSendNodes(compGridCommPkg)[i][j][k];
            }
         }
      }
   }

   if ( hypre_ParCompGridCommPkgNumRecvNodes(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgNumRecvNodes(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int**, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
      {
         HYPRE_Int num_comm_procs = hypre_ParCompGridCommPkgNumRecvProcs(compGridCommPkg)[i];
         hypre_ParCompGridCommPkgNumRecvNodes(copy_compGridCommPkg)[i] = hypre_CTAlloc(HYPRE_Int*, num_comm_procs, HYPRE_MEMORY_HOST);
         for (j = 0; j < num_comm_procs; j++)
         {
            hypre_ParCompGridCommPkgNumRecvNodes(copy_compGridCommPkg)[i][j] = hypre_CTAlloc(HYPRE_Int, num_levels, HYPRE_MEMORY_HOST);
            for (k = 0; k < num_levels; k++)
            {
               hypre_ParCompGridCommPkgNumRecvNodes(copy_compGridCommPkg)[i][j][k] = hypre_ParCompGridCommPkgNumRecvNodes(compGridCommPkg)[i][j][k];
            }
         }
      }
   }

   if ( hypre_ParCompGridCommPkgSendFlag(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgSendFlag(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int***, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
      {
         HYPRE_Int num_comm_procs = hypre_ParCompGridCommPkgNumSendProcs(compGridCommPkg)[i];
         hypre_ParCompGridCommPkgSendFlag(copy_compGridCommPkg)[i] = hypre_CTAlloc(HYPRE_Int**, num_comm_procs, HYPRE_MEMORY_HOST);
         for (j = 0; j < num_comm_procs; j++)
         {
            hypre_ParCompGridCommPkgSendFlag(copy_compGridCommPkg)[i][j] = hypre_CTAlloc(HYPRE_Int*, num_levels, HYPRE_MEMORY_HOST);
            for (k = 0; k < num_levels; k++)
            {
               if ( hypre_ParCompGridCommPkgSendFlag(compGridCommPkg)[i][j][k] )
               {                     
                  HYPRE_Int num_elmts = hypre_ParCompGridCommPkgNumSendNodes(compGridCommPkg)[i][j][k];
                  hypre_ParCompGridCommPkgSendFlag(copy_compGridCommPkg)[i][j][k] = hypre_CTAlloc(HYPRE_Int, num_elmts, HYPRE_MEMORY_HOST);
                  for (l = 0; l < num_elmts; l++)
                     hypre_ParCompGridCommPkgSendFlag(copy_compGridCommPkg)[i][j][k][l] = hypre_ParCompGridCommPkgSendFlag(compGridCommPkg)[i][j][k][l];
               }
            }
         }
      }
   }

   if ( hypre_ParCompGridCommPkgRecvMap(compGridCommPkg) )
   {
      hypre_ParCompGridCommPkgRecvMap(copy_compGridCommPkg) = hypre_CTAlloc(HYPRE_Int***, num_levels, HYPRE_MEMORY_HOST);
      for (i = 0; i < num_levels; i++)
      {
         HYPRE_Int num_comm_procs = hypre_ParCompGridCommPkgNumRecvProcs(compGridCommPkg)[i];
         hypre_ParCompGridCommPkgRecvMap(copy_compGridCommPkg)[i] = hypre_CTAlloc(HYPRE_Int**, num_comm_procs, HYPRE_MEMORY_HOST);
         for (j = 0; j < num_comm_procs; j++)
         {
            hypre_ParCompGridCommPkgRecvMap(copy_compGridCommPkg)[i][j] = hypre_CTAlloc(HYPRE_Int*, num_levels, HYPRE_MEMORY_HOST);
            for (k = 0; k < num_levels; k++)
            {
               if ( hypre_ParCompGridCommPkgRecvMap(compGridCommPkg)[i][j][k] )
               {                     
                  HYPRE_Int num_elmts = hypre_ParCompGridCommPkgNumRecvNodes(compGridCommPkg)[i][j][k];
                  hypre_ParCompGridCommPkgRecvMap(copy_compGridCommPkg)[i][j][k] = hypre_CTAlloc(HYPRE_Int, num_elmts, HYPRE_MEMORY_HOST);
                  for (l = 0; l < num_elmts; l++)
                     hypre_ParCompGridCommPkgRecvMap(copy_compGridCommPkg)[i][j][k][l] = hypre_ParCompGridCommPkgRecvMap(compGridCommPkg)[i][j][k][l];
               }
            }
         }
      }
   }

   return copy_compGridCommPkg;
}