#include "AntsParameters.h"

AntsParameters::AntsParameters()
{
   m_imageMetric_values.push_back("Cross-Correlation"); 
   m_imageMetric_values.push_back("Mutual Information"); 
   m_imageMetric_values.push_back("Probability Mapping"); 
   m_imageMetric_values.push_back("Mean Square Difference"); 
   m_imageMetric_default=getImageMetricFlag(m_imageMetric_values[0]);
   m_imageMetric1=m_imageMetric_default;
   m_imageMetric2=m_imageMetric_default;

   m_weight_min=0;
   //m_weight_max;
   m_weight_default=1; 
   m_weight1=m_weight_default;
   m_weight2=m_weight_default;

   m_radius_min=0;
   //m_radius_max;
   m_radius_default=4;
   m_radius1=m_radius_default;
   m_radius2=m_radius_default;

   m_iterations_min=0; 
   //m_iterations_max;
 
   m_iterationsJ_default=200;
   m_iterationsJ=m_iterationsJ_default;

   m_iterationsK_default=75; 
   m_iterationsK=m_iterationsK_default;

   m_iterationsL_default=50;
   m_iterationsL=m_iterationsL_default;

   m_transformationType_values.push_back("Symmetric Normalization");
   m_transformationType_values.push_back("Diffeomorphic");
   m_transformationType_values.push_back("Elastic");
   m_transformationType_values.push_back("Exponential Diff");
   m_tranformation_default=getTransformationTypeFlag(m_transformationType_values[0]);
   m_transformationType=m_tranformation_default;

   m_gradientStepLength_min=0;   
   //m_gradientStepLength_max;   
   m_gradientStepLength_default=0.125;   
   m_gradientStepLength=m_gradientStepLength_default;

   m_numberOfTimeSteps_min=0;
   //m_numberOfTimeSteps_max;
   m_numberOfTimeSteps_default=2;
   m_numberOfTimeSteps=m_numberOfTimeSteps_default;

   m_deltaTime_min=0;
   //m_deltaTime_max;
   m_deltaTime_default=0.01;
   m_deltaTime=m_deltaTime_default;

   m_regularizationType_values.push_back("Gaussian");
   m_regularizationType_values.push_back("Directly Manipulated");
   m_regularizationType_default=getRegularizationTypeFlag(m_regularizationType_values[1]);
   m_regularizationType=m_regularizationType_default;

   m_gradientFieldSigma_min=0;
   //m_gradientFieldSigma_max;
   m_gradientFieldSigma_default=3;
   m_gradientFieldSigma=m_gradientFieldSigma_default;

   m_deformationFieldSigma_min=0;
   //m_deformationFieldSigma_max;
   m_deformationFieldSigma_default=0;
   m_deformationFieldSigma=m_deformationFieldSigma_default;
 
   m_truncation_min=0; 
   //m_truncation_max; 
   m_truncation_default=0; 
   m_truncation=m_truncation_default; 

}

bool AntsParameters::isSuperior(int value, int min)
{
   if(value>min)
   {
      return true; 
   }
   return false; 
}

bool AntsParameters::isBetween(int value, int min, int max)
{
   if(value>min && value<max)
   {
      return true; 
   }
   return false; 
}

bool AntsParameters::isSuperior(double value, double min)
{
   if(value>min)
   {
      return true; 
   }
   return false; 
}

bool AntsParameters::isBetween(double value, double min, double max)
{
   if(value>min && value<max)
   {
      return true; 
   }
   return false; 
}


bool AntsParameters::isIn(QString item, std::vector<QString> vector)
{
   std::vector<QString>::iterator it; 
   for(it=vector.begin(); it!=vector.end(); ++it)
   {
      if(item.compare(*it, Qt::CaseInsensitive))
      {
         return true;
      }
   }
   return false; 
}



void AntsParameters::setImageMetric1(QString imageMetric1) {m_imageMetric1=getImageMetricFlag(imageMetric1);}
bool AntsParameters::checkImageMetric1(QString imageMetric1) {return isIn(imageMetric1, m_imageMetric_values);}
QString AntsParameters::getImageMetric1() {return m_imageMetric1;}

void AntsParameters::setImageMetric2(QString imageMetric2) {m_imageMetric2=getImageMetricFlag(imageMetric2);}
bool AntsParameters::checkImageMetric2(QString imageMetric2) {return isIn(imageMetric2, m_imageMetric_values);}
QString AntsParameters::getImageMetric2() {return m_imageMetric2;}


double AntsParameters::getWeightMin() {return m_weight_min;}

void AntsParameters::setWeight1(double weight1) {m_weight1=weight1;}
bool AntsParameters::checkWeight1(double weight1) {return isSuperior(weight1, m_weight_min);}
double AntsParameters::getWeight1() {return m_weight1;}

void AntsParameters::setWeight2(double weight2) {m_weight2=weight2;}
bool AntsParameters::checkWeight2(double weight2) {return isSuperior(weight2, m_weight_min);}
double AntsParameters::getWeight2() {return m_weight2;}


int AntsParameters::getRadiusMin() {return m_radius_min;}

void AntsParameters::setRadius1(int radius1) {m_radius1=radius1;}
bool AntsParameters::checkRadius1(int radius1) {return isSuperior(radius1, m_radius_min);}
int AntsParameters::getRadius1() {return m_radius1;}

void AntsParameters::setRadius2(int radius2) {m_radius2=radius2;}
bool AntsParameters::checkRadius2(int radius2) {return isSuperior(radius2, m_radius_min);}
int AntsParameters::getRadius2() {return m_radius2;}


int AntsParameters::getIterationsMin() {return m_iterations_min;}

void AntsParameters::setIterationsJ(int iterationsJ) {m_iterationsJ=iterationsJ;}
bool AntsParameters::checkIterationsJ(int iterationsJ) {return isSuperior(iterationsJ, m_iterationsJ_max);}
int AntsParameters::getIterationsJ() {return m_iterationsJ;}

void AntsParameters::setIterationsK(int iterationsK) {m_iterationsK=iterationsK;}
bool AntsParameters::checkIterationsK(int iterationsK) {return isSuperior(iterationsK, m_iterationsK_max);}
int AntsParameters::getIterationsK() {return m_iterationsK;}

void AntsParameters::setIterationsL(int iterationsL) {m_iterationsL=iterationsL;}
bool AntsParameters::checkIterationsL(int iterationsL) {return isSuperior(iterationsL, m_iterationsL_max);}
int AntsParameters::getIterationsL() {return m_iterationsL;}


void AntsParameters::setTransformationType(QString transformationType) {m_transformationType=getTransformationTypeFlag(transformationType);}
bool AntsParameters::checkTransformationType(QString transformationType) {return isIn(transformationType, m_transformationType_values);}
QString AntsParameters::getTransformationType() {return m_transformationType;}


void AntsParameters::setGradientStepLength(double gradientStepLength) {m_gradientStepLength=gradientStepLength;}
bool AntsParameters::checkGradientStepLength(double gradientStepLength) {return isSuperior(gradientStepLength, m_gradientStepLength_min);}
double AntsParameters::getGradientStepLength() {return m_gradientStepLength;}
double AntsParameters::getGradientStepLengthMin() {return m_gradientStepLength_min;}


void AntsParameters::setNumberOfTimeSteps(double numberOfTimeSteps) {m_numberOfTimeSteps=numberOfTimeSteps;}
bool AntsParameters::checkNumberOfTimeSteps(double numberOfTimeSteps) {return isSuperior(numberOfTimeSteps, m_numberOfTimeSteps_min);}
double AntsParameters::getNumberOfTimeSteps() {return m_numberOfTimeSteps;}
double AntsParameters::getNumberOfTimeStepsMin() {return m_numberOfTimeSteps_min;}


void AntsParameters::setDeltaTime(double deltaTime) {m_deltaTime=deltaTime;}
bool AntsParameters::checkDeltaTime(double deltaTime) {return isSuperior(deltaTime, m_deltaTime_min);}
double AntsParameters::getDeltaTime() {return m_deltaTime;}
double AntsParameters::getDeltaTimeMin() {return m_deltaTime_min;}


void AntsParameters::setRegularizationType(QString regularizationType) {m_regularizationType=getRegularizationTypeFlag(regularizationType);}
bool AntsParameters::checkRegularizationType(QString regularizationType) {return isIn(regularizationType, m_regularizationType_values);}
QString AntsParameters::getRegularizationType() {return m_regularizationType;}


void AntsParameters::setGradientFieldSigma(double gradientFieldSigma) {m_gradientFieldSigma=gradientFieldSigma;}
bool AntsParameters::checkGradientFieldSigma(double gradientFieldSigma) {return isSuperior(gradientFieldSigma, m_gradientFieldSigma_min);}
double AntsParameters::getGradientFieldSigma() {return m_gradientFieldSigma;}
double AntsParameters::getGradientFieldSigmaMin() {return m_gradientFieldSigma_min;}


void AntsParameters::setDeformationFieldSigma(double deformationFieldSigma) {m_deformationFieldSigma=deformationFieldSigma;}
bool AntsParameters::checkDeformationFieldSigma(double deformationFieldSigma) {return isSuperior(deformationFieldSigma, m_deformationFieldSigma_min);}
double AntsParameters::getDeformationFieldSigma() {return m_deformationFieldSigma;}
double AntsParameters::getDeformationFieldSigmaMin() {return m_deformationFieldSigma_min;}


void AntsParameters::setTruncation(double truncation) {m_truncation=truncation;}
bool AntsParameters::checkTruncation(double truncation) {return isSuperior(truncation, m_truncation_min);}
double AntsParameters::getTruncation() {return m_truncation;}
double AntsParameters::getTruncationMin() {return m_truncation_min;}


QString AntsParameters::getImageMetricFlag(QString imageMetric)
{
   if(imageMetric.compare("CC", Qt::CaseInsensitive) || imageMetric.compare("Cross-Correlation", Qt::CaseInsensitive) || imageMetric.compare("Cross Correlation", Qt::CaseInsensitive))
   {
      return "CC";
   }

   if(imageMetric.compare("MI", Qt::CaseInsensitive) || imageMetric.compare("Mutual Information", Qt::CaseInsensitive))
   {
      return "MI";
   }

   if(imageMetric.compare("PR", Qt::CaseInsensitive) || imageMetric.compare("Probability Mapping", Qt::CaseInsensitive))
   {
      return "PR";
   }

   if(imageMetric.compare("MSD", Qt::CaseInsensitive) || imageMetric.compare("Mean Square Difference", Qt::CaseInsensitive))
   {
      return "MSD";
   }

   return NULL;
} 


QString AntsParameters::getTransformationTypeFlag(QString transformationType)
{
   if(transformationType.compare("SyN", Qt::CaseInsensitive) || transformationType.compare("Symmetric Normalization", Qt::CaseInsensitive))
   {
      return "SyN";
   }

   if(transformationType.compare("Diff", Qt::CaseInsensitive) || transformationType.compare("Diffeomorphic", Qt::CaseInsensitive))
   {
      return "Diff";
   }

   if(transformationType.compare("Elast", Qt::CaseInsensitive) || transformationType.compare("Elastic", Qt::CaseInsensitive))
   {
      return "Elast";
   }

   if(transformationType.compare("Exp", Qt::CaseInsensitive) || transformationType.compare("Exponential Diff", Qt::CaseInsensitive))
   {
      return "Exp";
   }
   
   return NULL;
}


QString AntsParameters::getRegularizationTypeFlag(QString regularizationType)
{
   if(regularizationType.compare("Gauss", Qt::CaseInsensitive) || regularizationType.compare("Gaussian", Qt::CaseInsensitive))
   {
      return "Gauss";
   }

   if(regularizationType.compare("DMFFD", Qt::CaseInsensitive) || regularizationType.compare("Directly Manipulated", Qt::CaseInsensitive))
   {
      return "DMFFD";
   }
   
   return NULL;
}

