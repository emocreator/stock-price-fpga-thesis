#include "Data.h"

static const size_t NSEHISTORY_COLUMNS = 8;
static const size_t NSEHISTORY_FEATURES  = 7;
static const char* NSEHISTORY_CSV_FILENAME  = "data/NSE.csv";

DLL_EXPORT void DestroyObservationPool(struct ObservationPool* pool)
{
   for (size_t i = 0; i < pool->numberOfObservations; i++)
   {
      free(pool->observations[i].features);
   }
   free(pool->observations);
}

DLL_EXPORT void DestroyDataset(struct Dataset* dataset)
{
   free(dataset->observations);
}

DLL_EXPORT struct Dataset BuildRandomDataset(struct Dataset* dataset)
{
   struct Dataset newDataset;
   newDataset.numberOfFeatures = dataset->numberOfFeatures;
   newDataset.numberOfObservations = dataset->numberOfObservations;
   newDataset.observations = malloc(dataset->numberOfObservations * sizeof(struct Observation*));

   for (size_t i = 0; i < dataset->numberOfObservations; i++)
   {
      size_t randomIndex = rand() % (dataset->numberOfObservations);
      newDataset.observations[i] = dataset->observations[randomIndex];
   }

   return newDataset;
}

// Fisher-Yates (or Knuth) shuffle algorithm
void ShuffleObservationPool(struct ObservationPool* pool) 
{
   if (pool == NULL || pool->numberOfObservations <= 1) { return; }

   // Initialize random number generator
   srand((unsigned int)time(NULL));

   for (size_t i = pool->numberOfObservations - 1; i > 0; i--) 
   {
      // Generate a random index between 0 and i
      size_t j = rand() % (i + 1);

      // Swap features[i] with features[j]
      struct Observation tempObservation = pool->observations[i];
      pool->observations[i] = pool->observations[j];
      pool->observations[j] = tempObservation; 
   }
}

// Returns 0 on success
DLL_EXPORT int ImportNSEHistoryDataset(struct ObservationPool* pool)
{
   FILE *file = fopen(NSEHISTORY_CSV_FILENAME, "r");
   if (file == NULL) 
   { 
      perror("Unable to open file"); 
      return 1; 
   }

   // Setup ObservationPool for NSEHistory
   pool->numberOfObservations = 0;
   pool->numberOfFeatures = NSEHISTORY_FEATURES;

   // Get number of observations
   char line[1024];
   while (fgets(line, sizeof(line), file)) { pool->numberOfObservations++; }
   pool->numberOfObservations--; // Adjusting for header
   rewind(file); 

   // Allocate memory for observations
   pool->observations = malloc(pool->numberOfObservations * sizeof(struct Observation));
   if (!pool->observations) 
   {
      perror("Memory allocation failed");
      fclose(file);
      return 1;
   }

   // Store observations, focusing on all but the "Date" column
   size_t i = 0;
   fgets(line, sizeof(line), file); // Skip header
   while (fgets(line, sizeof(line), file))
   {
      pool->observations[i].features = malloc(NSEHISTORY_FEATURES * sizeof(double));
      if (!pool->observations[i].features) 
      {
         perror("Memory allocation failed");
         DestroyObservationPool(pool);
         fclose(file);
         return 1;
      }
      
      char *token = strtok(line, ",");
      for (size_t column = 0; column < NSEHISTORY_COLUMNS; column++)
      {
         if (!token) 
         {
            perror("Error parsing file");
            DestroyObservationPool(pool);
            fclose(file);
            return 1;
         }

         // Skip the "Date" column, start storing from the "Price" column
         if (column > 0) // This skips the first column (Date)
         {
            pool->observations[i].features[column - 1] = strtod(token, NULL);
         }
         token = strtok(NULL, ","); // Get next token
      }
      i++;
   }

   // Close file
   fclose(file);

   // Randomize dataset
   ShuffleObservationPool(pool);

   return 0;
}


// Splits dataset into training data and testing data
DLL_EXPORT void SplitPool(const struct ObservationPool* pool, 
                          struct Dataset* train,
                          struct Dataset* test,
                          const double fractionTraining)
{
   if (fractionTraining > 1.0 || fractionTraining < 0) { return; }

   // Calculate training and test dataset sizes
   const size_t trainingSize = (size_t)(fractionTraining * (double)(pool->numberOfObservations));
   const size_t testSize = pool->numberOfObservations - trainingSize;

   // Setup new datasets
   train->numberOfObservations = trainingSize; 
   test->numberOfObservations = testSize;
   train->numberOfFeatures = pool->numberOfFeatures;
   test->numberOfFeatures = pool->numberOfFeatures;
   train->observations = malloc(trainingSize * sizeof(struct Observation*));
   test->observations = malloc(testSize * sizeof(struct Observation*));

   // Copy data from full dataset to train and test datasets
   for (size_t i = 0; i < trainingSize; i++)
   {
      train->observations[i] = &pool->observations[i];
   }

   size_t datasetIndex = trainingSize;
   for (size_t i = 0; i < testSize; i++)
   {
      test->observations[i] = &pool->observations[datasetIndex++];
   }
}

DLL_EXPORT void PrintObservationPool(const struct ObservationPool* pool)
{
   for (size_t i = 0; i < pool->numberOfObservations; i++)
   {
      printf("\nrow %zu:", i);
      for (size_t j = 0; j < pool->numberOfFeatures; j++)
      {
         printf(" %f,", pool->observations[i].features[j]);
      }
   }
}


DLL_EXPORT void PrintDataset(struct Dataset* dataset)
{
   for (size_t i = 0; i < dataset->numberOfObservations; i++)
   {
      printf("\nrow %zu:", i);
      for (size_t j = 0; j < dataset->numberOfFeatures; j++)
      {
         printf(" %f,", dataset->observations[i]->features[j]);
      }
   }
}

size_t* GetRandomFeatureIndices(const struct Dataset* dataset,
                                const size_t numberOfFeatures)
{
   if (numberOfFeatures > dataset->numberOfFeatures) { return NULL; }

   // Create array of all indices
   size_t* allIndices = malloc(dataset->numberOfFeatures * sizeof(size_t));
   for (size_t i = 0; i < dataset->numberOfFeatures; i++)
   {
      allIndices[i] = i;
   }

   // Shuffle indices array
   for (size_t i = 0; i < dataset->numberOfFeatures; i++)
   {
      size_t j = rand() % dataset->numberOfFeatures;
      size_t temp = allIndices[i];
      allIndices[i] = allIndices[j];
      allIndices[j] = temp;
   }

   // Choose the first n indices from allIndices and return
   size_t* featureIndices = malloc(numberOfFeatures * sizeof(size_t));
   for (size_t i = 0 ; i < numberOfFeatures; i++)
   {
      featureIndices[i] = allIndices[i];
   }
   
   free(allIndices);
   return featureIndices;
}
