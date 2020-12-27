
#ifndef JARGON_MATH_LINEARFUNCTION_H
#define JARGON_MATH_LINEARFUNCTION_H


namespace Jargon{
namespace Math{

	/**
		A simple linear function in the form
			f(x) = mx + b;
	*/
	template<class Tx, class Tm = Tx, class Tb = Tx>
	class LinearFunction{
		public:
			LinearFunction():
				m(),
				b()
			{
			}

			LinearFunction(const Tm & m, const Tb & b):
				m(m),
				b(b)
			{
			}

			Tx operator()(const Tx & x) const{
				return apply(x);
			}

			Tx apply(const Tx & x) const{
				return m*x + b;
			}

			Tm getM() const{
				return m;
			}

			Tb getB() const{
				return b;
			}

			void setM(const Tm & m){
				this->m = m;
			}

			void setB(const Tb & b){
				this->b = b;
			}

		private:
			Tm m;
			Tb b;

	};

}
}

#endif
