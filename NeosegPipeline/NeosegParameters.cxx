#include "NeosegParameters.h"

NeosegParameters::NeosegParameters()
{
   m_referenceImage_values << "T1" << "T2";  
   m_referenceImage_default = m_referenceImage_values[0];
   m_referenceImage = m_referenceImage_default;

   m_filterMethod_values << "curvature flow" << "grad aniso diffusion"; 
   m_filterMethod_default = m_filterMethod_values[0];
   m_filterMethod = m_filterMethod_default;

   m_filterIterations_min = 0;
   m_filterIterations_default = 5;
   m_filterIterations = m_filterIterations_default;

   m_filterTimeStep_lowerBound = 0;
   m_filterTimeStep_default = 0.01;   
   m_filterTimeStep = m_filterTimeStep_default;

   m_priorThreshold_lowerBound = 0;
   m_priorThreshold_default = 0.9;
   m_priorThreshold = m_priorThreshold_default;

   m_maxBiasDegree_min = 2;
   m_maxBiasDegree_default = 4;
   m_maxBiasDegree = m_maxBiasDegree_default;

   m_prior_lowerBound = 0 ;

   m_prior1_default = 0.4;
   m_prior1 = m_prior1_default;

   m_prior2_default = 1.4;
   m_prior2 = m_prior2_default;

   m_prior3_default = 1;
   m_prior3 = m_prior3_default;

   m_prior4_default = 0.3;
   m_prior4 = m_prior4_default;

   m_prior5_default = 1;
   m_prior5 = m_prior5_default;

   m_refinement_default = false; 
   m_refinement = m_refinement_default;

   m_initialParzenKernelWidth_lowerBound = 0;
   m_initialParzenKernelWidth_default = 0.05;
   m_initialParzenKernelWidth = m_initialParzenKernelWidth_default;
}



bool NeosegParameters::isIn(QString item, QStringList list)
{
   QStringList::iterator it; 
   for(it=list.begin(); it!=list.end(); ++it)
   {
      if(item.compare(*it, Qt::CaseInsensitive))
      {
         return true;
      }
   }
   return false; 
}

// Reference image
bool NeosegParameters::checkReferenceImage(QString referenceImage)
{
   return isIn(referenceImage, m_referenceImage_values);
}
void NeosegParameters::setReferenceImage(QString referenceImage)
{
   m_referenceImage=referenceImage;
}
QString NeosegParameters::getReferenceImage()
{
   return m_referenceImage;
}
int NeosegParameters::getReferenceImageIndex()
{
   return m_referenceImage_values.indexOf(m_referenceImage); 
}
QStringList NeosegParameters::getReferenceImageValues()
{
   return m_referenceImage_values; 
}


// Filter method 
bool NeosegParameters::checkFilterMethod(QString filterMethod)
{
   return isIn(filterMethod, m_filterMethod_values);
}
void NeosegParameters::setFilterMethod(QString filterMethod)
{
   m_filterMethod=filterMethod;
}
QString NeosegParameters::getFilterMethod()
{
   return m_filterMethod;
}
int NeosegParameters::getFilterMethodIndex()
{
   return m_filterMethod_values.indexOf(m_filterMethod); 
}
QStringList NeosegParameters::getFilterMethodValues()
{
   return m_filterMethod_values; 
}

// Number of iterations
bool NeosegParameters::checkNumberOfIterations(int filterIterations)
{
   return filterIterations >= m_filterIterations_min ;
}
void NeosegParameters::setNumberOfIterations(int filterIterations)
{
   m_filterIterations=filterIterations;
}
int NeosegParameters::getNumberOfIterations()
{
   return m_filterIterations;
}


// Time step
bool NeosegParameters::checkTimeStep(double filterTimeStep)
{
   return filterTimeStep > m_filterTimeStep_lowerBound ;
}
void NeosegParameters::setTimeStep(double filterTimeStep)
{
   m_filterTimeStep=filterTimeStep;
}
double NeosegParameters::getTimeStep()
{
   return m_filterTimeStep;
}


// Prior threshold 
bool NeosegParameters::checkPriorThreshold(double priorThreshold)
{
   return priorThreshold > m_priorThreshold_lowerBound ;
}
void NeosegParameters::setPriorThreshold(double priorThreshold)
{
   m_priorThreshold=priorThreshold;
}
double NeosegParameters::getPriorThreshold()
{
   return m_priorThreshold;
}

// Max bias degree
bool NeosegParameters::checkMaxBiasDegree(int maxBiasDegree)
{
   return maxBiasDegree >= m_maxBiasDegree_min ;
}
void NeosegParameters::setMaxBiasDegree(int maxBiasDegree)
{
   m_maxBiasDegree=maxBiasDegree;
}
int NeosegParameters::getMaxBiasDegree()
{
   return m_maxBiasDegree;
}

// Prior 1
bool NeosegParameters::checkPrior1(double prior1)
{
   return prior1 > m_prior_lowerBound ;
}
void NeosegParameters::setPrior1(double prior1)
{
   m_prior1=prior1;
}
double NeosegParameters::getPrior1()
{
   return m_prior1;
}

// Prior 2 
bool NeosegParameters::checkPrior2(double prior2)
{
   return prior2 > m_prior_lowerBound ;
}
void NeosegParameters::setPrior2(double prior2)
{
   m_prior2=prior2;
}
double NeosegParameters::getPrior2()
{
   return m_prior2;
}

// Prior 3 
bool NeosegParameters::checkPrior3(double prior3)
{
   return prior3 > m_prior_lowerBound ;
}
void NeosegParameters::setPrior3(double prior3)
{
   m_prior3=prior3;
}
double NeosegParameters::getPrior3()
{
   return m_prior3;
}

// Prior 4
bool NeosegParameters::checkPrior4(double prior4)
{
   return prior4 > m_prior_lowerBound ;
}
void NeosegParameters::setPrior4(double prior4)
{
   m_prior4=prior4;
}
double NeosegParameters::getPrior4()
{
   return m_prior4;
}

// Prior 5 
bool NeosegParameters::checkPrior5(double prior5)
{
   return prior5 > m_prior_lowerBound ;
}
void NeosegParameters::setPrior5(double prior5)
{
   m_prior5=prior5;
}
double NeosegParameters::getPrior5()
{
   return m_prior5;
}

// Refinement 
void NeosegParameters::setRefinement(bool refinement)
{
   m_refinement=refinement;
}
bool NeosegParameters::getRefinement()
{
   return m_refinement;
}

// Initial parzen kernal width
bool NeosegParameters::checkInitialParzenKernelWidth(double initialParzenKernelWidth)
{
   return initialParzenKernelWidth > m_initialParzenKernelWidth_lowerBound ;
}
void NeosegParameters::setInitialParzenKernelWidth(double initialParzenKernelWidth)
{
   m_initialParzenKernelWidth=initialParzenKernelWidth;
}
double NeosegParameters::getInitialParzenKernelWidth()
{
   return m_initialParzenKernelWidth;
}


