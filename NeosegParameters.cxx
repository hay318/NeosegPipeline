#include "NeosegParameters.h"

NeosegParameters::NeosegParameters()
{
   m_filterMethod_values.push_back("curvature flow"); 
   m_filterMethod_values.push_back("grad aniso diffusion"); 
   m_filterMethod_default=m_filterMethod_values[0];
   m_filterMethod=m_filterMethod_default;

   m_filterIterations_min=0;
   //m_filterIterations_max;
   m_filterIterations_default=5;
   m_filterIterations=m_filterIterations_default;

   m_filterTimeStep_min=0;
   //m_filterTimeStep_max;
   m_filterTimeStep_default=0.01;   
   m_filterTimeStep=m_filterTimeStep_default;

   m_referenceImage_values.push_back("T1"); 
   m_referenceImage_values.push_back("T2"); 
   m_referenceImage_default=m_referenceImage_values[0];
   m_referenceImage=m_referenceImage_default;

   m_priorThreshold_min=0;
   m_priorThreshold_max=1;
   m_priorThreshold_default=0.9;
   m_priorThreshold=m_priorThreshold_default;

   m_maxBiasDegree_min=2;
   //m_maxBiasDegree_max;
   m_maxBiasDegree_default=4;
   m_maxBiasDegree=m_maxBiasDegree_default;

   m_prior_min=0; 
   //m_prior_max; 

   m_prior1_default=0.4;
   m_prior1=m_prior1_default;

   m_prior2_default=1.4;
   m_prior2=m_prior2_default;

   m_prior3_default=1;
   m_prior3=m_prior3_default;

   m_prior4_default=1;
   m_prior4=m_prior4_default;

   m_prior5_default=1;
   m_prior5=m_prior5_default;

   m_refinement_default=false; 
   m_refinement=m_refinement_default;

   m_initialParzenKernelWidth_min=0;
   //m_initialParzenKernelWidth_max;
   m_initialParzenKernelWidth_default=0.05;
   m_initialParzenKernelWidth=m_initialParzenKernelWidth_default;
}

bool NeosegParameters::isSuperior(int value, int min)
{
   if(value>min)
   {
      return true; 
   }
   return false; 
}

bool NeosegParameters::isBetween(int value, int min, int max)
{
   if(value>min && value<max)
   {
      return true; 
   }
   return false; 
}

bool NeosegParameters::isSuperior(double value, double min)
{
   if(value>min)
   {
      return true; 
   }
   return false; 
}

bool NeosegParameters::isBetween(double value, double min, double max)
{
   if(value>min && value<max)
   {
      return true; 
   }
   return false; 
}


bool NeosegParameters::isIn(QString item, std::vector<QString> vector)
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




bool NeosegParameters::checkFilterMethod(QString filterMethod) {return isIn(filterMethod, m_filterMethod_values);}

bool NeosegParameters::checkNumberOfIterations(int filterIterations) {return isSuperior(filterIterations, m_filterIterations_min);}

bool NeosegParameters::checkTimeStep(double filterTimeStep) {return isSuperior(filterTimeStep, m_filterTimeStep_min);}

bool NeosegParameters::checkReferenceImage(QString referenceImage) {return isIn(referenceImage, m_referenceImage_values);}

bool NeosegParameters::checkPriorThreshold(double priorThreshold) {return isBetween(priorThreshold, m_priorThreshold_min, m_priorThreshold_max);}

bool NeosegParameters::checkMaxBiasDegree(int maxBiasDegree) {return isSuperior(maxBiasDegree, m_maxBiasDegree_min);}

bool NeosegParameters::checkPrior1(double prior1) {return isSuperior(prior1, m_prior_min);}

bool NeosegParameters::checkPrior2(double prior2) {return isSuperior(prior2, m_prior_min);}

bool NeosegParameters::checkPrior3(double prior3) {return isSuperior(prior3, m_prior_min);}

bool NeosegParameters::checkPrior4(double prior4) {return isSuperior(prior4, m_prior_min);}

bool NeosegParameters::checkPrior5(double prior5) {return isSuperior(prior5, m_prior_min);}

bool NeosegParameters::checkInitialParzenKernelWidth(double initialParzenKernelWidth) {return isSuperior(initialParzenKernelWidth, m_initialParzenKernelWidth_min);}


void NeosegParameters::setFilterMethod(QString filterMethod) {m_filterMethod=filterMethod;}

void NeosegParameters::setNumberOfIterations(int filterIterations) {m_filterIterations=filterIterations;}

void NeosegParameters::setTimeStep(double filterTimeStep) {m_filterTimeStep=filterTimeStep;}

void NeosegParameters::setReferenceImage(QString referenceImage) {m_referenceImage=referenceImage;}

void NeosegParameters::setPriorThreshold(double priorThreshold) {m_priorThreshold=priorThreshold; }

void NeosegParameters::setMaxBiasDegree(int maxBiasDegree) {m_maxBiasDegree=maxBiasDegree; }

void NeosegParameters::setPrior1(double prior1) {m_prior1=prior1;}

void NeosegParameters::setPrior2(double prior2) {m_prior2=prior2;}

void NeosegParameters::setPrior3(double prior3) {m_prior3=prior3;}

void NeosegParameters::setPrior4(double prior4) {m_prior4=prior4;}

void NeosegParameters::setPrior5(double prior5) {m_prior5=prior5;}

void NeosegParameters::setRefinement(bool refinement) {m_refinement=refinement;}

void NeosegParameters::setInitialParzenKernelWidth(double initialParzenKernelWidth) {m_initialParzenKernelWidth=initialParzenKernelWidth;}


QString NeosegParameters::getFilterMethod() {return m_filterMethod;}

int NeosegParameters::getNumberOfIterations() {return m_filterIterations;}

double NeosegParameters::getTimeStep() {return m_filterTimeStep;}

QString NeosegParameters::getReferenceImage() {return m_referenceImage;}

double NeosegParameters::getPriorThreshold() {return m_priorThreshold;}

int NeosegParameters::getMaxBiasDegree() {return m_maxBiasDegree;}

double NeosegParameters::getPrior1() {return m_prior1;}

double NeosegParameters::getPrior2() {return m_prior2;}

double NeosegParameters::getPrior3() {return m_prior3;}

double NeosegParameters::getPrior4() {return m_prior4;}

double NeosegParameters::getPrior5() {return m_prior5;}

bool NeosegParameters::getRefinement() {return m_refinement;}

double NeosegParameters::getInitialParzenKernelWidth() {return m_initialParzenKernelWidth;}



