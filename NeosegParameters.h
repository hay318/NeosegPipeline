#ifndef DEF_NeosegParameters
#define DEF_NeosegParameters

#include <vector>

#include <QString>

class NeosegParameters
{
   public:

      NeosegParameters();

      bool isSuperior(int value, int min);
      bool isSuperior(double value, double min);

      bool isBetween(int value, int min, int max);
      bool isBetween(double value, double min, double max);
   
      bool isIn(QString item, std::vector<QString> vector);

      bool checkFilterMethod(QString filterMethod);
      bool checkNumberOfIterations(int filterIterations);
      bool checkTimeStep(double filterTimeStep);

      bool checkReferenceImage(QString referenceImage);
      bool checkPriorThreshold(double priorThreshold);
      bool checkMaxBiasDegree(int maxBiasDegree);
      bool checkPrior1(double prior1);
      bool checkPrior2(double prior2);
      bool checkPrior3(double prior3);
      bool checkPrior4(double prior4);
      bool checkPrior5(double prior5);

      bool checkInitialParzenKernelWidth(double initialParzenKernelWidth);


      void setFilterMethod(QString filterMethod);
      void setNumberOfIterations(int filterIterations);
      void setTimeStep(double filterTimeStep);

      void setReferenceImage(QString referenceImage);
      void setPriorThreshold(double priorThreshold);
      void setMaxBiasDegree(int maxBiasDegree);
      void setPrior1(double prior1);
      void setPrior2(double prior2);
      void setPrior3(double prior3);
      void setPrior4(double prior4);
      void setPrior5(double prior5);

      void setRefinement(bool refinement);
      void setInitialParzenKernelWidth(double initialParzenKernelWidth);

      QString getFilterMethod();
      int getNumberOfIterations();
      double getTimeStep();

      QString getReferenceImage();
      double getPriorThreshold();
      int getMaxBiasDegree();
      double getPrior1();
      double getPrior2();
      double getPrior3();
      double getPrior4();
      double getPrior5();

      bool getRefinement();
      double getInitialParzenKernelWidth();


   private:

      std::vector<QString> m_filterMethod_values; 
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

      std::vector<QString> m_referenceImage_values; 
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

