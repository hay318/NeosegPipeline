#ifndef DEF_NeosegParameters
#define DEF_NeosegParameters

// Genaral Librairies
#include <stdlib.h>
#include <iostream>

// Qt Librairies
#include <QString>
#include <QMap>

class NeosegParameters
{
   public:

   // Constructor 
   NeosegParameters();

   // Test
   bool isSuperior(int value, int min);
   bool isSuperior(double value, double min);

   bool isBetween(int value, int min, int max);
   bool isBetween(double value, double min, double max);
   
   bool isIn(QString item, QMap<QString, int> vector);

   // Reference image 
   bool checkReferenceImage(QString referenceImage);
   void setReferenceImage(QString referenceImage);
   QString getReferenceImage();
   int getReferenceImageIndex();
   
   // Filter method 
   void setFilterMethod(QString filterMethod);
   bool checkFilterMethod(QString filterMethod);
   QString getFilterMethod();
   int getFilterMethodIndex();

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



   private:

   QMap<QString, int> m_filterMethod_values; 
   QString m_filterMethod_default;
   QString m_filterMethod;

   int m_filterIterations_min;
   int m_filterIterations_max;
   int m_filterIterations_default;
   int m_filterIterations;

   double m_filterTimeStep_min;
   double m_filterTimeStep_max;
   double m_filterTimeStep_default;   
   double m_filterTimeStep;

   QMap<QString, int> m_referenceImage_values; 
   QString m_referenceImage_default;
   QString m_referenceImage;


   double m_priorThreshold_min;
   double m_priorThreshold_max;
   double m_priorThreshold_default;
   double m_priorThreshold;

   int m_maxBiasDegree_default;
   int m_maxBiasDegree_min;
   int m_maxBiasDegree_max;
   int m_maxBiasDegree;

   double m_prior_min; 
   double m_prior_max; 

   double m_prior1_default;
   double m_prior1;

   double m_prior2_default;
   double m_prior2;

   double m_prior3_default;
   double m_prior3;

   double m_prior4_default;
   double m_prior4;

   double m_prior5_default;
   double m_prior5;

   bool m_refinement_default; 
   bool m_refinement;

   double m_initialParzenKernelWidth_min;
   double m_initialParzenKernelWidth_max;
   double m_initialParzenKernelWidth_default;
   double m_initialParzenKernelWidth;

};

#endif

