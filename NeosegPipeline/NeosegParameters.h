#ifndef DEF_NeosegParameters
#define DEF_NeosegParameters

// Genaral Librairies
#include <stdlib.h>
#include <iostream>

// Qt Librairies
#include <QString>
#include <QStringList>


class NeosegParameters
{
   public:

   // Constructor 
   NeosegParameters();

   // Test
   bool isIn(QString item, QStringList list);

   // Reference image 
   bool checkReferenceImage(QString referenceImage);
   void setReferenceImage(QString referenceImage);
   QString getReferenceImage();
   int getReferenceImageIndex();
   QStringList getReferenceImageValues();
   
   // Filter method 
   void setFilterMethod(QString filterMethod);
   bool checkFilterMethod(QString filterMethod);
   QString getFilterMethod();
   int getFilterMethodIndex();
   QStringList getFilterMethodValues();

   // Number of iterations
   bool checkNumberOfIterations(int filterIterations);
   void setNumberOfIterations(int filterIterations);
   int getNumberOfIterations();

   // Time step 
   bool checkTimeStep(double filterTimeStep);
   void setTimeStep(double filterTimeStep);
   double getTimeStep();

   // Prior threshold 
   bool checkPriorThreshold(double priorThreshold);
   void setPriorThreshold(double priorThreshold);
   double getPriorThreshold();

   // Max bias degree 
   bool checkMaxBiasDegree(int maxBiasDegree);
   void setMaxBiasDegree(int maxBiasDegree);
   int getMaxBiasDegree();

   // Prior 1
   bool checkPrior1(double prior1);
   void setPrior1(double prior1);
   double getPrior1();

   // Prior 2   
   bool checkPrior2(double prior2);
   void setPrior2(double prior2);
   double getPrior2();

   // Prior 3
   bool checkPrior3(double prior3);
   void setPrior3(double prior3);
   double getPrior3();

   // Prior 4 
   bool checkPrior4(double prior4);
   void setPrior4(double prior4);
   double getPrior4();

   // Prior 5 
   bool checkPrior5(double prior5);
   void setPrior5(double prior5);
   double getPrior5();

   // Refinement
   void setRefinement(bool refinement);
   bool getRefinement();

   // Initial parzen kernel width
   bool checkInitialParzenKernelWidth(double initialParzenKernelWidth);
   void setInitialParzenKernelWidth(double initialParzenKernelWidth);
   double getInitialParzenKernelWidth();

   //Check ABC values paramters

   bool checkInitialDistributionEstimator(QString item);
   QStringList getInitialDistributionEstimatorValues();

   private:

   // Reference Image 
   QStringList m_referenceImage_values; 
   QString m_referenceImage_default;
   QString m_referenceImage;

   // Filter Method
   QStringList m_filterMethod_values; 
   QString m_filterMethod_default;
   QString m_filterMethod;

   // Filter Iterations
   int m_filterIterations_min;
   int m_filterIterations_default;
   int m_filterIterations;

   // filter Time Step 
   double m_filterTimeStep_lowerBound;
   double m_filterTimeStep_default;   
   double m_filterTimeStep;

   // Prior Threshold
   double m_priorThreshold_lowerBound;
   double m_priorThreshold_default;
   double m_priorThreshold;

   // Max Bias Degree
   int m_maxBiasDegree_default;
   int m_maxBiasDegree_min;
   int m_maxBiasDegree;

   // Priors 
   double m_prior_lowerBound;

   // Prior1
   double m_prior1_default;
   double m_prior1;

   // Prior 2
   double m_prior2_default;
   double m_prior2;

   // Prior 3
   double m_prior3_default;
   double m_prior3;

   // Prior 4
   double m_prior4_default;
   double m_prior4;

   // Prior 5
   double m_prior5_default;
   double m_prior5;

   // Refinement
   bool m_refinement_default; 
   bool m_refinement;

   // Initial Parzen Kernel Width
   double m_initialParzenKernelWidth_lowerBound;
   double m_initialParzenKernelWidth_default;
   double m_initialParzenKernelWidth;

   QStringList m_initialDistributionEstimatorValues;

};

#endif

