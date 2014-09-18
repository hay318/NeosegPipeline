#ifndef __itkSpreadFA_h
#define __itkSpreadFA_h

namespace itk
{
   //namespace Functor
   //{
      template< class InputPixelType, class OutputPixelType >
      class SpreadFA
      {
         public:
            SpreadFA() {}
            ~SpreadFA() {}

            void SetShift(double shift)
            {
               m_Shift = shift;
            }

            void SetSigma(double sigma)
            {
               m_Sigma = sigma;
            }

            bool operator!=(const SpreadFA & other) const
            {
               return false;
            }

            bool operator==(const SpreadFA & other) const
            {
               return !( *this != other );
            }

            inline OutputPixelType operator()(const InputPixelType & x) const
            {
               OutputPixelType result = (erf((x/255-m_Shift)*m_Sigma)+1)/2;
               return result;
            }

         private:
            double m_Shift;
            double m_Sigma;
      };
   //}
}

#endif
