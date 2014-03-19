#ifndef DEF_AntsParameters
#define DEF_AntsParameters

#include <vector>

#include <QString>

class AntsParameters
{
   public:

      AntsParameters();

      bool isSuperior(int value, int min);
      bool isSuperior(double value, double min);

      bool isBetween(int value, int min, int max);
      bool isBetween(double value, double min, double max);
   
      bool isIn(QString item, std::vector<QString> vector);


      void setImageMetric1(QString imageMetric1);
      bool checkImageMetric1(QString imageMetric1);
      QString getImageMetric1();

      void setImageMetric2(QString imageMetric2);
      bool checkImageMetric2(QString imageMetric2);
      QString getImageMetric2();


      double getWeightMin();

      void setWeight1(double weight1);
      bool checkWeight1(double weight1);
      double getWeight1();

      void setWeight2(double weight2);
      bool checkWeight2(double weight2);
      double getWeight2();


      int getRadiusMin();

      void setRadius1(int radius1);
      bool checkRadius1(int radius1);
      int getRadius1();

      void setRadius2(int radius2);
      bool checkRadius2(int radius2);
      int getRadius2();


      int getIterationsMin();    
  
      void setIterationsJ(int iterationJ);
      bool checkIterationsJ(int iterationsJ);
      int getIterationsJ();

      void setIterationsK(int iterationK);
      bool checkIterationsK(int iterationsK);
      int getIterationsK();

      void setIterationsL(int iterationL);
      bool checkIterationsL(int iterationsL);
      int getIterationsL();


      void setTransformationType(QString transformationType);
      bool checkTransformationType(QString transformationType);
      QString getTransformationType();

      double getGradientStepLengthMin();
      void setGradientStepLength(double gradientStepLength);
      bool checkGradientStepLength(double gradientStepLength);
      double getGradientStepLength();

      double getNumberOfTimeStepsMin();
      void setNumberOfTimeSteps(double numberOfTimeSteps);
      bool checkNumberOfTimeSteps(double numberOfTimeSteps);
      double getNumberOfTimeSteps();

      double getDeltaTimeMin();
      void setDeltaTime(double deltaTime);
      bool checkDeltaTime(double deltaTime);
      double getDeltaTime();


      void setRegularizationType(QString regularizationType);
      bool checkRegularizationType(QString regularizationType);
      QString getRegularizationType();

      double getGradientFieldSigmaMin();
      void setGradientFieldSigma(double gradientFieldSigma);
      bool checkGradientFieldSigma(double gradientFieldSigma);
      double getGradientFieldSigma();

      double getDeformationFieldSigmaMin();
      void setDeformationFieldSigma(double deformationFieldSigma);
      bool checkDeformationFieldSigma(double deformationFieldSigma);
      double getDeformationFieldSigma();


      QString getImageMetricFlag(QString imageMetric);
      QString getTransformationTypeFlag(QString transformationType); 
      QString getRegularizationTypeFlag(QString regularizationType); 


   private:
   
      std::vector<QString> m_imageMetric_values; 
      QString m_imageMetric_default;
      QString m_imageMetric1;
      QString m_imageMetric2;

      double m_weight_min;
      double m_weight_max;
      double m_weight_default; 
      double m_weight1;
      double m_weight2;

      int m_radius_min;
      int m_radius_max;
      int m_radius_default;
      int m_radius1;
      int m_radius2;

      int m_iterations_min; 
      int m_iterationsJ_max; 
      int m_iterationsJ_default;
      int m_iterationsJ;

      int m_iterationsK_min; 
      int m_iterationsK_max; 
      int m_iterationsK_default; 
      int m_iterationsK;

      int m_iterationsL_min; 
      int m_iterationsL_max; 
      int m_iterationsL_default;
      int m_iterationsL;

      std::vector<QString> m_transformationType_values;
      QString m_tranformation_default;
      QString m_transformationType;

      double m_gradientStepLength_min;   
      double m_gradientStepLength_max;   
      double m_gradientStepLength_default;   
      double m_gradientStepLength;

      double m_numberOfTimeSteps_min;
      double m_numberOfTimeSteps_max;
      double m_numberOfTimeSteps_default;
      double m_numberOfTimeSteps;

      double m_deltaTime_min;
      double m_deltaTime_max;
      double m_deltaTime_default;
      double m_deltaTime;

      std::vector<QString> m_regularizationType_values;
      QString m_regularizationType_default;
      QString m_regularizationType;

      double m_gradientFieldSigma_min;
      double m_gradientFieldSigma_max;
      double m_gradientFieldSigma_default;
      double m_gradientFieldSigma;

      double m_deformationFieldSigma_min;
      double m_deformationFieldSigma_max;
      double m_deformationFieldSigma_default;
      double m_deformationFieldSigma;
}; 


#endif
