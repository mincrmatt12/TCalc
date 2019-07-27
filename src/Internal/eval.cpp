#include "eval.hpp"
#include "lcd12864_charset.hpp"
#include "unitconv.hpp"
#include "usart.hpp"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CONST_PI 3.14159265358979323846
#define CONST_E 2.71828182845904523536
#define CONST_AVOGADRO 6.022140758e23
#define CONST_ELEMCHG 1.60217662e-19
#define CONST_VLIGHT 299792458
#define CONST_AGRAV 9.80665

#define DEG_TO_RAD(deg) ((deg) * CONST_PI / 180.0)
#define RAD_TO_DEG(rad) ((rad) * 180.0 / CONST_PI)
#define TRIG_FUNC_INPUT(x) (useRadians ? (x) : DEG_TO_RAD(x))
#define TRIG_FUNC_OUTPUT(x) (useRadians ? (x) : RAD_TO_DEG(x))

namespace eval {
	
	bool useRadians = true;
    bool autoFractions = true;

	/******************** Number ********************/
	Number* Number::constFromString(const char* str) {
		if(strcmp(str, LCD_STR_PI) == 0) {
			return new Number(CONST_PI);
		}
		else if(strcmp(str, LCD_STR_EULR) == 0) {
			return new Number(CONST_E);
		}
		else if(strcmp(str, LCD_STR_AVGO) == 0) {
			return new Number(CONST_AVOGADRO);
		}
		else if(strcmp(str, LCD_STR_ECHG) == 0) {
			return new Number(CONST_ELEMCHG);
		}
		else if(strcmp(str, LCD_STR_VLIG) == 0) {
			return new Number(CONST_VLIGHT);
		}
		else if(strcmp(str, LCD_STR_AGV) == 0) {
			return new Number(CONST_AGRAV);
		}
		else {
			return nullptr;
		}
	}

	/******************** Fraction ********************/
	int64_t Fraction::gcd(int64_t a, int64_t b) {
		while(true) {
			if(a == 0) {
				return b;
			}
			if(b == 0) {
				return a;
			}

			int64_t r = a % b;
			a = b;
			b = r;
		}
	}
	int64_t Fraction::lcm(int64_t a, int64_t b) {
		return (a * b) / gcd(a, b);
	}
	double Fraction::doubleVal() const {
		// Make sure they're cast to doubles first to avoid integer division
		return static_cast<double>(num) / static_cast<double>(denom);
	}
	bool Fraction::isInteger() const {
		return num % denom == 0;
	}
	void Fraction::reduce() {
		// Make sure the denominator is always positive
		if(denom < 0) {
			num *= -1;
			denom *= -1;
		}

		// Now that the denominator is positive, we can make sure the result we get is also positive
		int64_t divisor = labs(gcd(num, denom));
		if(divisor == 1) {
			return;
		}
		num /= divisor;
		denom /= divisor;
	}
	Fraction& Fraction::operator+=(const Fraction &frac) {
		int64_t newDenom = lcm(denom, frac.denom);
		int64_t numA = num * (newDenom / denom);
		int64_t numB = frac.num * (newDenom / frac.denom);
		num = numA + numB;
		denom = newDenom;
		reduce();
		return *this;
	}
	Fraction& Fraction::operator-=(const Fraction &frac) {
		int64_t newDenom = lcm(denom, frac.denom);
		int64_t numA = num * (newDenom / denom);
		int64_t numB = frac.num * (newDenom / frac.denom);
		num = numA - numB;
		denom = newDenom;
		reduce();
		return *this;
	}
	Fraction& Fraction::operator*=(const Fraction &frac) {
		num *= frac.num;
		denom *= frac.denom;
		reduce();
		return *this;
	}
	Fraction& Fraction::operator/=(const Fraction &frac) {
		num *= frac.denom;
		denom *= frac.num;
		reduce();
		return *this;
	}
	bool Fraction::pow(const Fraction &other) {
		double e = other.doubleVal();
		double n = ::pow(num, e);
		// Check if the numerator and denominator are still ints
		if(!isInt(n)) {
			return false;
		}
		double d = ::pow(denom, e);
		if(!isInt(d)) {
			return false;
		}

		num = static_cast<int64_t>(n);
		denom = static_cast<int64_t>(d);
		reduce();
		return true;
	}

	/******************** Matrix ********************/
	Matrix* Matrix::add(const Matrix &a, const Matrix &b) {
		// Make sure two matrices are the same size
		if(a.m != b.m || a.n != b.n) {
			return nullptr;
		}

		Matrix *result = new Matrix(a.m, a.n);
		for(uint16_t i = 0; i < a.m * a.n; i ++) {
			(*result)[i] = a[i] + b[i];
		}
		return result;
	}
	Matrix* Matrix::subtract(const Matrix &a, const Matrix &b) {
		// Make sure two matrices are the same size
		if(a.m != b.m || a.n != b.n) {
			return nullptr;
		}

		Matrix *result = new Matrix(a.m, a.n);
		for(uint16_t i = 0; i < a.m * a.n; i ++) {
			(*result)[i] = a[i] - b[i];
		}
		return result;
	}
	Matrix* Matrix::multiply(const Matrix &a, const Matrix &b) {
		// Make sure the two matrices can be multiplied with each other
		if(a.n != b.m) {
			return nullptr;
		}

		Matrix *result = new Matrix(a.m, b.n);
		for(uint8_t row = 0; row < a.m; row ++) {
			for(uint8_t col = 0; col < b.n; col ++) {
				// Take the dot product
				double sum = 0;
				for(uint8_t i = 0; i < a.n; i ++) {
					sum += a.getEntry(row, i) * b.getEntry(i, col);
				}
				result->setEntry(row, col, sum);
			}
		}
		return result;
	}
	Matrix* Matrix::multiply(const Matrix &a, double scalar) {
		Matrix *result = new Matrix(a.m, a.n);
		for(uint16_t i = 0; i < a.m * a.n; i ++) {
			(*result)[i] = a[i] * scalar;
		}
		return result;
	}
	double Matrix::dot(const Matrix &a, const Matrix &b) {
		if(a.n == 1 && b.n == 1 && a.m == b.m) {
			double sum = 0;
			for(uint8_t i = 0; i < a.m; i ++) {
				sum += a[i] * b[i];
			}
			return sum;
		}
		else {
			return NAN;
		}
	}
	double Matrix::det() {
		// No determinant for nonsquare matrices
		if(m != n) {
			return NAN;
		}
		
        // Apply gaussian elimination to make the matrix upper-triangular
        // Keep track of the negating
        bool neg = 0;
        // Copied from eliminate() and modified
        for(uint8_t i = 0; i < m; i ++) {
            if(getEntry(i, i) == 0) {
                uint8_t k = i;
                for(; k < m; k ++) {
                    if(getEntry(k, i) != 0) {
                        rowSwap(i, k);
                        // Swapping two rows negates the determinant
                        neg = -neg;
                        break;
                    }
                }

                if(k == m) {
                    // If the entire column is 0, the matrix is singular
                    // Therefore the determinant must be 0
                    return 0;
                }
            }

            double pivot = getEntry(i, i);

            // Eliminate this column in all rows below
            // Adding to one row a scalar multiple of another does not change the determinant
            for(uint8_t k = i + 1; k < m; k ++) {
                rowAdd(k, i, -(getEntry(k, i) / pivot));
            }
        }

        // Now the matrix should be upper-triangular
        // Take the product of the main diagonal to get the determinant
        double d = 1;
        for(uint8_t i = 0; i < m; i ++) {
            d *= getEntry(i, i);
        }

        return neg ? -d : d;
	}
	double Matrix::len() const {
		if(n != 1) {
			return NAN;
		}
		double sum = 0;
		for(uint8_t i = 0; i < m; i ++) {
			sum += contents[i] * contents[i];
		}
		return sqrt(sum);
	}
	Matrix* Matrix::cross(const Matrix &a, const Matrix &b) {
		// Only supported for 3d vectors
		if(a.m != 3 || a.n != 1 || b.m != 3 || b.n != 1) {
			return nullptr;
		}
		
		Matrix *result = new Matrix(3, 1);
		(*result)[0] = a[1] * b[2] - a[2] * b[1];
		(*result)[1] = a[2] * b[0] - a[0] * b[2];
		(*result)[2] = a[0] * b[1] - a[1] * b[0];
		return result;
	}
	Matrix* Matrix::transpose() const {
		Matrix *result = new Matrix(n, m);
		
		for(uint8_t i = 0; i < m; i ++) {
			for(uint8_t j = 0; j < n; j ++) {
				result->setEntry(j, i, getEntry(i, j));
			}
		}
		return result;
	}
	bool Matrix::eliminate(bool allowSingular) {
		// If there are more rows than columns, don't do anything
		if(n < m && !allowSingular) {
			return false;
		}

        for(uint8_t i = 0, j = 0; i < m && j < n; j ++) {
            if(getEntry(i, j) == 0) {
                uint8_t k = i;
                for(; k < m; k ++) {
                    if(getEntry(k, j) != 0) {
                        rowSwap(i, k);
                        break;
                    }
                }

                if(k == m) {
                    if(!allowSingular) {
                        return false;
                    }
                    continue;
                }
            }

            rowMult(i, 1 / getEntry(i, j));
            for(uint8_t k = 0; k < m; k ++) {
                if(i == k) {
                    continue;
                }

                rowAdd(k, i, -getEntry(k, j));
            }
            
            i ++;
        }

		return true;
	}
	Matrix* Matrix::inv() const {
		// Nonsquare matrices have no inverses
		if(m != n) {
			return nullptr;
		}
		// Create block matrix [A|I], where I is the identity matrix
		Matrix block(m, n + m);
		// Copy the original matrix
		for(uint8_t i = 0; i < m; i ++) {
			for(uint8_t j = 0; j < n; j ++) {
				block.setEntry(i, j, getEntry(i, j));
			}
		}
		// Copy the identity matrix
		for(uint8_t i = 0; i < m; i ++) {
			block.setEntry(i, i + m, 1);
		}
		// Eliminate
		if(!block.eliminate(false)) {
			return nullptr;
		}
		// Copy inverse
		Matrix *result = new Matrix(m, n);
		for(uint8_t i = 0; i < m; i ++) {
			for(uint8_t j = 0; j < n; j ++) {
				result->setEntry(i, j, block.getEntry(i, j + m));
			}
		}
		return result;
	}
    Matrix* Matrix::getRowVector(uint8_t row) const {
        if(row >= m) {
            return nullptr;
        }

        Matrix *mat = new Matrix(1, n);
        for(uint8_t i = 0; i < n; i ++) {
            mat->contents[i] = getEntry(row, i);
        }
        return mat;
    }
    Matrix* Matrix::getColVector(uint8_t col) const {
        if(col >= n) {
            return nullptr;
        }
        
        Matrix *mat = new Matrix(m, 1);
        for(uint8_t i = 0; i < m; i ++) {
            mat->contents[i] = getEntry(i, col);
        }
        return mat;
    }

	/******************** Operator ********************/
	uint8_t Operator::getPrecedence() const {
		switch(type) {
		case Type::SP_MULT:
		case Type::SP_DIV:
			return 0;
        case Type::TRANSPOSE:
        case Type::INVERSE:
            return 1;
		case Type::EXPONENT:
			return 2;
        /*
         * Note: 
         * Although factorial should have a higher precedence than exponentiation, ie an expression like this
         * a^b!
         * should be equivalent to
         * a^(b!)
         * and not
         * (a^b)!
         * here it's defined the opposite way. 
         * 
         * This is because in TCalc, an expression like this
         *  b
         * a !
         * is translated directly into
         * a^b!
         * If the factorial operator were to have higher precedence than the exponent operator, this would not make sense,
         * as the factorial clearly applies to the entire expression a^b.
         */
        case Type::FACT:
            return 3;
        case Type::NOT:
        case Type::NEGATE:
            return 4;
        case Type::AUGMENT:
            return 5;
		case Type::MULTIPLY:
		case Type::DIVIDE:
		case Type::CROSS:
			return 6;
		case Type::PLUS:
		case Type::MINUS:
			return 7;
		case Type::EQUALITY:
        case Type::NOT_EQUAL:
        case Type::LT:
        case Type::GT:
        case Type::LTEQ:
        case Type::GTEQ:
			return 8;
        case Type::AND:
            return 9;
        case Type::OR:
            return 10;
        case Type::XOR:
            return 11;
		
		default: return 0xFF;
		}
	}
    bool Operator::isUnary() const {
        switch(type) {
        case Type::NOT:
        case Type::NEGATE:
        case Type::FACT:
        case Type::TRANSPOSE:
        case Type::INVERSE:
            return true;
        }
        return false;
    }
	const Operator* Operator::fromChar(char ch) {
		switch(ch) {
		case '+':
			return &OP_PLUS;
			
		case '-':
			return &OP_MINUS;

		case LCD_CHAR_MUL:
		case '*':
			return &OP_MULTIPLY;

		case LCD_CHAR_DIV:
		case '/':
			return &OP_DIVIDE;
        
		case LCD_CHAR_CRS:
			return &OP_CROSS;
        
        case '>':
            return &OP_GT;
        
        case '<':
            return &OP_LT;

        case LCD_CHAR_GEQ:
            return &OP_GTEQ;
        
        case LCD_CHAR_LEQ:
            return &OP_LTEQ;
        
        case LCD_CHAR_LAND:
            return &OP_AND;

        case LCD_CHAR_LOR:
            return &OP_OR;
        
        case LCD_CHAR_LXOR:
            return &OP_XOR;
        
        case LCD_CHAR_LNOT:
            return &OP_NOT;
        
        case '!':
            return &OP_FACT;
        
        case '|':
            return &OP_AUGMENT;

		default: return nullptr;
		}
	}
	double Operator::operate(double lhs, double rhs) const {
		switch(type) {
		case Type::PLUS:
			return lhs + rhs;

		case Type::MINUS:
			return lhs - rhs;

		case Type::SP_MULT:
		case Type::MULTIPLY:
		// In the case of two scalars, just use it as if it's a normal multiplication
		case Type::CROSS:
			return lhs * rhs;

		case Type::SP_DIV:
		case Type::DIVIDE:
			return lhs / rhs;

		case Type::EXPONENT:
			return pow(lhs, rhs);

		case Type::EQUALITY:
			return floatEq(lhs, rhs);
        
        case Type::NOT_EQUAL:
            return !floatEq(lhs, rhs);

        case Type::GT:
            return lhs > rhs;

        case Type::LT:
            return lhs < rhs;

        case Type::GTEQ:
            return lhs > rhs || floatEq(lhs, rhs);

        case Type::LTEQ:
            return lhs < rhs || floatEq(lhs, rhs);

        case Type::AND:
        {
            Number l(lhs);
            Number r(rhs);
            int8_t lTruthy = isTruthy(&l);
            int8_t rTruthy = isTruthy(&r);
            // If the truthiness of either operand is undefined, the expression is undefined
            if(lTruthy == -1 || rTruthy == -1) {
                return NAN;
            } 
            // Now we know they can only be 0 or 1, we can use regular logical operators
            return lTruthy && rTruthy;
        }

        case Type::OR:
        {
            Number l(lhs);
            Number r(rhs);
            int8_t lTruthy = isTruthy(&l);
            int8_t rTruthy = isTruthy(&r);
            // If the truthiness of either operand is undefined, the expression is undefined
            if(lTruthy == -1 || rTruthy == -1) {
                return NAN;
            } 

            return lTruthy || rTruthy;
        }

        case Type::XOR:
        {
            Number l(lhs);
            Number r(rhs);
            int8_t lTruthy = isTruthy(&l);
            int8_t rTruthy = isTruthy(&r);
            // If the truthiness of either operand is undefined, the expression is undefined
            if(lTruthy == -1 || rTruthy == -1) {
                return NAN;
            }

            return lTruthy ^ rTruthy;
        }

		default: return NAN;
		}
	}
	bool Operator::operateOn(Fraction *frac, Fraction *rhs) const {
		switch(type) {
		case Type::PLUS:
			*frac += *rhs;
			break;

		case Type::MINUS:
			*frac -= *rhs;
			break;
		
		case Type::SP_MULT:
		case Type::MULTIPLY:
		case Type::CROSS:
			*frac *= *rhs;
			break;
		
		case Type::SP_DIV:
		case Type::DIVIDE:
			*frac /= *rhs;
			break;
		
		case Type::EXPONENT:
			return frac->pow(*rhs);
    
		default: return false;
		}
		return true;
	}
	// Operates on two tokens and returns the result
	// Note: This also deletes the operands!
	Token* Operator::operator()(Token *lhs, Token *rhs) const {
		// TODO: Matrix processing
		TokenType lType = lhs->getType();
		TokenType rType = rhs->getType();
		Token *result = nullptr;
		// Two numbers: normal operation
		if(lType == TokenType::NUMBER && rType == TokenType::NUMBER) {
			// Special case for division: if the operands are whole numbers, create a fraction if auto fractions is on
			if(autoFractions && (type == Operator::Type::DIVIDE || type == Operator::Type::SP_DIV) && 
                    isInt(((Number*) lhs)->value) && isInt(((Number*) rhs)->value)) {
				auto n = static_cast<int64_t>(((Number*) lhs)->value);
				auto d = static_cast<int64_t>(((Number*) rhs)->value);
				// See if the division yields a whole number
				// Watch out for division by zero!
				if(d == 0) {
					if(n == 0) {
						result = new Number(NAN);
					}
					else {
						result = new Number(n > 0 ? INFINITY : -INFINITY);
					}
				}
				else if(n % d == 0) {
					// If the result is an integer, just push the integer instead
					result = new Number(n / d);
				}
				else {
					// Otherwise create a fraction
					result = new Fraction(n, d);
				}
			}
			else {
				result = new Number(operate(((Number*) lhs)->value, ((Number*) rhs)->value));
			}
			delete lhs;
			delete rhs;
		}
		// Two fractions: fraction operation
		else if(lType == TokenType::FRACTION && rType == TokenType::FRACTION) {
			// Record if action was successful
			bool success = operateOn((Fraction*) lhs, (Fraction*) rhs);
			if(success) {
				// See if result is an integer
				if(((Fraction*) lhs)->isInteger()) {
					// If yes then directly insert a number
					result = new Number(((Fraction*) lhs)->doubleVal());
					delete lhs;
				}
				else {
					result = lhs;
				}
				delete rhs;
			}
			// If the operation was not possible, convert to double and operate normally
			else {
				result = new Number(operate(((Fraction*) lhs)->doubleVal(), ((Fraction*) rhs)->doubleVal()));
				delete lhs;
				delete rhs;
			}
		}
		// One fraction: fraction operation if the other one is integer, normal operation if not
		else if(lType == TokenType::FRACTION && rType == TokenType::NUMBER) {
			// Test if rhs is integer
			if(isInt(((Number*) rhs)->value)) {
                // This operation is not guaranteed to succeed
				Fraction rhsFrac((int64_t) ((Number*) rhs)->value, 1);
				bool success = operateOn((Fraction*) lhs, &rhsFrac);
				if(success) {
					if(static_cast<Fraction*>(lhs)->isInteger()) {
						result = new Number(static_cast<Fraction*>(lhs)->doubleVal());
                        delete lhs;
					}
					else {
						result = lhs;
					}
					delete rhs;
				}
				else {
					// My code is terrible, indeed.
					goto convertToDoubleAndOperate;
				}
			}
			// Otherwise convert to doubles
			else {
				result = new Number(operate(((Fraction*) lhs)->doubleVal(), ((Number*) rhs)->value));
				delete lhs;
				delete rhs;
			}
		}
		// One fraction: fraction operation if the other one is integer, normal operation if not
		else if(lType == TokenType::NUMBER && rType == TokenType::FRACTION) {
			if(isInt(((Number*) lhs)->value)) {
				// This operation is not guaranteed to succeed
				// Construct fraction since it's not going to be temporary if this operation succeeds
				Fraction *lhsFrac = new Fraction((int64_t) ((Number*) lhs)->value, 1);
				bool success = operateOn(lhsFrac, (Fraction*) rhs);
				if(success) {
					if(lhsFrac->isInteger()) {
						result = new Number(lhsFrac->doubleVal());
						delete lhsFrac;
					}
					else {
						result = lhsFrac;
					}
					delete lhs;
					delete rhs;
				}
				else {
					delete lhsFrac;
					// My code is terrible, indeed.
					goto convertToDoubleAndOperate;
				}
			}
			else {
convertToDoubleAndOperate:
				result = new Number(operate(((Number*) lhs)->value, ((Fraction*) rhs)->doubleVal()));
				delete lhs;
				delete rhs;
			}
		}
		// Two matrices
		else if(lType == TokenType::MATRIX && rType == TokenType::MATRIX) {
			Matrix *lMat = (Matrix*) lhs;
			Matrix *rMat = (Matrix*) rhs;
			
			switch(type) {
			case Type::PLUS:
				result = Matrix::add(*lMat, *rMat);
				// If not possible return NAN
				if(!result) {
					result = new Number(NAN);
				}
				break;
			case Type::MINUS:
				result = Matrix::subtract(*lMat, *rMat);
				// If not possible return NAN
				if(!result) {
					result = new Number(NAN);
				}
				break;
			case Type::MULTIPLY:
				result = Matrix::multiply(*lMat, *rMat);
				if(!result) {
					// If regular multiplication is not possible, see if the dot product can be computed
					// Since if the dot product isn't possible, dot will return NAN, we can directly return the result of the call
					result = new Number(Matrix::dot(*lMat, *rMat));
				}
				break;
			case Type::CROSS:
				result = Matrix::cross(*lMat, *rMat);
				if(!result) {
					result = new Number(NAN);
				}
				break;
            case Type::EQUALITY:
            {
                if(lMat->m != rMat->m || lMat->n != rMat->n) {
                    result = new Number(0);
                    break;
                }
                bool equal = true;
                for(uint16_t i = 0; i < lMat->m * lMat->n; i ++) {
                    if(!floatEq((*lMat)[i], (*rMat)[i])) {
                        equal = false;
                        break;
                    }
                }
                result = new Number(equal);
                break;
            }
            case Type::NOT_EQUAL:
            {
                if(lMat->m != rMat->m || lMat->n != rMat->n) {
                    result = new Number(1);
                    break;
                }
                bool equal = true;
                for(uint16_t i = 0; i < lMat->m * lMat->n; i ++) {
                    if(!floatEq((*lMat)[i], (*rMat)[i])) {
                        equal = false;
                        break;
                    }
                }
                result = new Number(!equal);
                break;
            }
            case Type::AUGMENT:
            {
                if(lMat->m != rMat->m) {
                    result = new Number(NAN);
                    break;
                }
                Matrix *mat = new Matrix(lMat->m, lMat->n + rMat->n);
                for(uint8_t i = 0; i < mat->m; i ++) {
                    for(uint8_t j = 0; j < mat->n; j ++) {
                        if(j < lMat->n) {
                            mat->setEntry(i, j, lMat->getEntry(i, j));
                        }
                        else {
                            mat->setEntry(i, j, rMat->getEntry(i, j - lMat->n));
                        }
                    }
                }
                result = mat;
                break;
            }
			default:
				result = new Number(NAN);
				break;
			}

			delete lhs;
			delete rhs;
		}
		// Matrix and scalar
		else if(lType == TokenType::MATRIX && rType == TokenType::NUMBER || rType == TokenType::FRACTION) {
			Matrix *lMat = (Matrix*) lhs;
			double rDouble = extractDouble(rhs);
			
			switch(type) {
			case Type::MULTIPLY:
			case Type::CROSS:
				result = Matrix::multiply(*lMat, rDouble);
				break;
			case Type::DIVIDE:
				result = Matrix::multiply(*lMat, 1.0 / rDouble);
				break;
            case Type::EQUALITY:
                result = new Number(0);
                break;
            case Type::NOT_EQUAL:
                result = new Number(1);
                break;
			default:
				result = new Number(NAN);
				break;
			}

			delete lhs;
			delete rhs;
		}
		else if((lType == TokenType::NUMBER || lType == TokenType::FRACTION) && TokenType::MATRIX) {
			double lDouble = extractDouble(lhs);
			Matrix *rMat = (Matrix*) rhs;
			
			switch(type) {
			case Type::MULTIPLY:
			case Type::CROSS:
				result = Matrix::multiply(*rMat, lDouble);
				break;
            case Type::EQUALITY:
                result = new Number(0);
                break;
            case Type::NOT_EQUAL:
                result = new Number(1);
                break;
			default:
				result = new Number(NAN);
				break;
			}

			delete lhs;
			delete rhs;
		}
		else {
			delete lhs;
			delete rhs;
		}
		return result;
	}
    Token* Operator::operator()(Token *t) const {
        switch(type) {
        case Type::NOT:
        {
            int8_t truthy = isTruthy(t);
            delete t;

            if(truthy == 1) {
                return new Number(0);
            }
            else if(truthy == 0) {
                return new Number(1);
            }
            // Undefined
            else {
                return new Number(NAN);
            }
        }
        case Type::NEGATE:
        {
            if(t->getType() == TokenType::NUMBER) {
                static_cast<Number*>(t)->value *= -1;
            }
            else if(t->getType() == TokenType::FRACTION) {
                static_cast<Fraction*>(t)->num *= -1;
            }
            // Matrix
            else {
                for(uint16_t i = 0; i < static_cast<Matrix*>(t)->m * static_cast<Matrix*>(t)->n; i ++) {
                    // Negate every entry
                    (*static_cast<Matrix*>(t))[i] *= -1;
                }
            }
            return t;
        }
        case Type::FACT:
        {
            double x = extractDouble(t);
            delete t;
			if(!isInt(x) || x < 0) {
				return new Number(NAN);
			}
			double d = 1;
			while(x > 0) {
				d *= x;
				--x;
			}
			return new Number(d);
        }
        case Type::TRANSPOSE:
		{
            if(t->getType() != TokenType::MATRIX) {
                return nullptr;
            }
			Matrix *mat = static_cast<Matrix*>(t);
			Matrix *result = mat->transpose();
            delete t;
            return result;
		}
        case Type::INVERSE:
		{
			if(t->getType() != TokenType::MATRIX) {
                return nullptr;
            }
			Matrix *mat = static_cast<Matrix*>(t);
			Matrix *result = mat->inv();
            delete t;
			
			return result ? (Token*) result : (Token*) new Number(NAN);
		}
        default: 
            return nullptr;
        }
    }

	/******************** Function ********************/
	// Must be in the same order as type
	const char * const Function::FUNCNAMES[TYPE_COUNT] = {
		"sin", "cos", "tan", "asin", "acos", "atan", "sinh", "cosh", "tanh", "asinh",
        "acosh", "atanh", "ln", 
		// log10 and log2 cannot be directly entered with a string
		"\xff", "\xff",

		"qdRtA", "qdRtB", "round", "min", "max", "floor", "ceil", "det", "linSolve", "rref",
	};
    const char * const Function::FUNC_FULLNAMES[TYPE_COUNT_DISPLAYABLE] = {
        "sin(angle)", "cos(angle)", "tan(angle)", "asin(x)", "acos(x)", "atan(x)", 
        "sinh(angle)", "cosh(angle)", "tanh(angle)", "asinh(x)", "acosh(x)", "atanh(x)",
        "ln(x)", "qdRtA(a,b,c)", "qdRtB(a,b,c)", "round(n,decimals)", "min(a,b)",
        "max(a,b)", "floor(x)", "ceil(x)", "det(A)", "linSolve(A)", "rref(A)"
    };
	Function* Function::fromString(const char *str) {
		for(uint8_t i = 0; i < TYPE_COUNT; i ++) {
			if(strcmp(str, FUNCNAMES[i]) == 0) {
				return new Function(static_cast<Type>(i));
			}
		}
		return nullptr;
	}
	uint8_t Function::getNumArgs() const {
		switch(type) {
		case Type::QUADROOT_A:
		case Type::QUADROOT_B:
			return 3;
		case Type::ROUND:
        case Type::MAX:
        case Type::MIN:
			return 2;
		default: 
			return 1;
		}
	}
	Token* Function::operator()(Token **args) const {
		// Since extractDouble returns NAN if the input is a matrix, functions that don't support matrices
		// will simply return NAN
		switch(type) {
		case Type::SIN:
		{
			return new Number(sin(TRIG_FUNC_INPUT(extractDouble(args[0]))));
		}
		case Type::COS:
		{
			return new Number(cos(TRIG_FUNC_INPUT(extractDouble(args[0]))));
		}
		case Type::TAN:
		{
			return new Number(tan(TRIG_FUNC_INPUT(extractDouble(args[0]))));
		}
		case Type::ASIN:
		{
			return new Number(TRIG_FUNC_OUTPUT(asin(extractDouble(args[0]))));
		}
		case Type::ACOS:
		{
			return new Number(TRIG_FUNC_OUTPUT(acos(extractDouble(args[0]))));
		}
		case Type::ATAN:
		{
			return new Number(TRIG_FUNC_OUTPUT(atan(extractDouble(args[0]))));
		}
		case Type::LN:
		{
			return new Number(log(extractDouble(args[0])));
		}
		case Type::LOG10:
		{
			return new Number(log10(extractDouble(args[0])));
		}
		case Type::LOG2:
		{
			return new Number(log2(extractDouble(args[0])));
		}
		case Type::SINH:
		{
			return new Number(sinh(TRIG_FUNC_INPUT(extractDouble(args[0]))));
		}
		case Type::COSH:
		{
			return new Number(cosh(TRIG_FUNC_INPUT(extractDouble(args[0]))));
		}
		case Type::TANH:
		{
			return new Number(tanh(TRIG_FUNC_INPUT(extractDouble(args[0]))));
		}
		case Type::ASINH:
		{
			return new Number(TRIG_FUNC_OUTPUT(asinh(extractDouble(args[0]))));
		}
		case Type::ACOSH:
		{
			return new Number(TRIG_FUNC_OUTPUT(acosh(extractDouble(args[0]))));
		}
		case Type::ATANH:
		{
			return new Number(TRIG_FUNC_OUTPUT(atanh(extractDouble(args[0]))));
		}
		case Type::QUADROOT_A:
		{
			double a = extractDouble(args[0]), b = extractDouble(args[1]), c = extractDouble(args[2]);
			return new Number((-b + sqrt(b * b - 4 * a * c)) / (2 * a));
		}
		case Type::QUADROOT_B:
		{
			double a = extractDouble(args[0]), b = extractDouble(args[1]), c = extractDouble(args[2]);
			return new Number((-b - sqrt(b * b - 4 * a * c)) / (2 * a));
		}
		case Type::ROUND:
		{
			if(!isInt(extractDouble(args[1]))) {
				return new Number(NAN);
			}
			return new Number(round(extractDouble(args[0]), extractDouble(args[1])));
		}
		case Type::DET:
		{
			// Syntax error: determinant of a scalar??
			if(args[0]->getType() != TokenType::MATRIX) {
				return nullptr;
			}
			Matrix *mat = (Matrix*) args[0];
			
			return new Number(mat->det());
		}
		case Type::LINSOLVE:
		{
			// Syntax error: can't solve a scalar or a matrix of the wrong dimensions
			if(args[0]->getType() != TokenType::MATRIX || ((Matrix*) args[0])->n != ((Matrix*) args[0])->m + 1) {
				return nullptr;
			}
			Matrix *mat = (Matrix*) args[0];
			if(!mat->eliminate(false)) {
				return new Number(NAN);
			}
			// Construct solution as vector
			Matrix *solution = new Matrix(mat->m, 1);
			for(uint8_t i = 0; i < mat->m; i ++) {
				(*solution)[i] = mat->getEntry(i, mat->m);
			}
			return solution;
		}
        case Type::RREF:
        {
            // Syntax error: rref of a scalar
			if(args[0]->getType() != TokenType::MATRIX) {
				return nullptr;
			}

            // Create a copy of the matrix
            // This is because the args may be freed in the future
            Matrix *mat = new Matrix(*static_cast<Matrix*>(args[0]));
            mat->eliminate(true);
            return mat;
        }
        case Type::MIN:
        {
            if(args[0]->getType() == TokenType::MATRIX || args[1]->getType() == TokenType::MATRIX) {
                return nullptr;
            }
            // a is greater than b
            if(compareTokens(args[0], args[1]) > 0) {
                // Make a copy of b
                if(args[1]->getType() == TokenType::NUMBER) {
                    return new Number(*static_cast<Number*>(args[1]));
                }
                else {
                    return new Fraction(*static_cast<Fraction*>(args[1]));
                }
            }
            else {
                if(args[0]->getType() == TokenType::NUMBER) {
                    return new Number(*static_cast<Number*>(args[0]));
                }
                else {
                    return new Fraction(*static_cast<Fraction*>(args[0]));
                }
            }
        }
        case Type::MAX:
        {
            if(args[0]->getType() == TokenType::MATRIX || args[1]->getType() == TokenType::MATRIX) {
                return nullptr;
            }
            // a is less than b
            if(compareTokens(args[0], args[1]) < 0) {
                // Make a copy of b
                if(args[1]->getType() == TokenType::NUMBER) {
                    return new Number(*static_cast<Number*>(args[1]));
                }
                else {
                    return new Fraction(*static_cast<Fraction*>(args[1]));
                }
            }
            else {
                if(args[0]->getType() == TokenType::NUMBER) {
                    return new Number(*static_cast<Number*>(args[0]));
                }
                else {
                    return new Fraction(*static_cast<Fraction*>(args[0]));
                }
            }
        }
        case Type::FLOOR:
        {
            if(args[0]->getType() == TokenType::MATRIX) {
                return nullptr;
            }
            return new Number(floor(extractDouble(args[0])));
        }
        case Type::CEIL:
        {
            if(args[0]->getType() == TokenType::MATRIX) {
                return nullptr;
            }
            return new Number(ceil(extractDouble(args[0])));
        }
		default: return new Number(NAN);
		}
	}

	/******************** Other Functions ********************/
	bool isDigit(char ch) {
		return (ch >= '0' && ch <= '9') || ch == '.' || ch == LCD_CHAR_EE;
	}
    // Checks if a character is a valid character for a variable/constant name.
	bool isNameChar(char ch) {
        // First check if it's a letter
        if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
            return true;
        }
        // Check if it's a special character
        else if(ch >= LCD_CHARSET_LOWBOUND && ch <= LCD_CHARSET_HIGHBOUND) {
            // These special characters cannot be used in names
            switch(ch) {
            case LCD_CHAR_EE:
            case LCD_CHAR_MUL:
            case LCD_CHAR_DIV:
            case LCD_CHAR_CRS:
            case LCD_CHAR_LEQ:
            case LCD_CHAR_GEQ:
            case LCD_CHAR_SERR:
            case LCD_CHAR_LAND:
            case LCD_CHAR_LOR:
            case LCD_CHAR_LNOT:
            case LCD_CHAR_LXOR:
            case LCD_CHAR_RARW:
                return false;
            default: return true;
            }
        }
        // If neither, return false
        else {
            return false;
        }
	}
	char extractChar(const neda::NEDAObj *obj) {
		if(obj->getType() != neda::ObjType::CHAR_TYPE) {
			return '\0';
		}
		return ((neda::Character*) obj)->ch;
	}
	// Returns the double value of a Token
	// The token must be a number or fraction. Otherwise NaN will be returned.
	double extractDouble(const Token *t) {
		return t->getType() == TokenType::NUMBER ? ((Number*) t)->value 
				: (t->getType() == TokenType::FRACTION ? ((Fraction*) t)->doubleVal() : NAN);
	}
	// Returns positive if a > b, zero if equal, and negative if a < b
	int8_t compareTokens(const Token *a, const Token *b) {
		double aVal = extractDouble(a);
		double bVal = extractDouble(b);
		return aVal > bVal ? 1 : bVal > aVal ? -1 : 0;
	}
	uint16_t findEquals(const DynamicArray<neda::NEDAObj*> &arr, bool forceVarName) {
		uint16_t equalsIndex = 0;
		bool validName = true;
		for(; equalsIndex < arr.length(); ++equalsIndex) {
			// Search for equals
			if(extractChar(arr[equalsIndex]) == '=') {
				// At the same time make sure it's not a == or a !=
                if(equalsIndex != 0 && extractChar(arr[equalsIndex - 1]) == '!') {
                    continue;
                }
				if(equalsIndex + 1 < arr.length() && extractChar(arr[equalsIndex + 1]) == '=') {
					++equalsIndex;
					continue;
				}
				else {
					break;
				}
			}
			if(forceVarName && validName) {
				// Check for name validity only if forceVarName is true
				// In addition to finding the equals sign, also verify that the left hand side of the equals only contains valid
				// name characters
                // Allow variable names containing numbers such as a1 and a2, but the number must not be the first character
                char ch = extractChar(arr[equalsIndex]);
                bool nc = isNameChar(ch);
                if((equalsIndex == 0 && !nc) || !(nc || (ch >= '0' && ch <= '9'))) {
					validName = false;
					break;
                }
			}
		}
		// If equalsIndex is the same as the length, then the if condition was never true, return an error value
		// Or if it's at index 0 or length - 1, return null since the condition can't be complete
		// Or if the name is not valid
		if(!validName || equalsIndex == arr.length() || equalsIndex == 0 || equalsIndex == arr.length() - 1) {
			return 0xFFFF;
		}
		return equalsIndex;
	}
    /*
     * Tests to see if a value is "truthy".
     * 
     * "Truthy" values are nonzero numbers or fractions, or any matrix/vector.
     * NaNs and infinities are undefined.
     * 
     * Returns 1 if truthy, 0 if not, -1 if undefined.
     */
    int8_t isTruthy(const Token *token) {
        if(token->getType() == TokenType::MATRIX) {
            return 1;
        }
        double v = token->getType() == TokenType::NUMBER ? static_cast<const Number*>(token)->value 
                : static_cast<const Fraction*>(token)->doubleVal();
        
        // Infinite or NaN
        if(!isfinite(v)) {
            return -1;
        }
        
        return v == 0 ? 0 : 1;
    }
    // This will delete the collection of tokens properly. It will destory all tokens in the array.
	void freeTokens(Deque<Token*> &q) {
		while (!q.isEmpty()) {
			Token *t = q.dequeue();
			if (t->getType() == TokenType::MATRIX || t->getType() == TokenType::NUMBER || t->getType() == TokenType::FRACTION || t->getType() == TokenType::FUNCTION) {
				delete t;
			}
		}
	}
    // This will delete the collection of tokens properly. It will destory all tokens in the array.
	void freeTokens(DynamicArray<Token*> &q) {
		for(Token *t : q) {
			if (t->getType() == TokenType::MATRIX || t->getType() == TokenType::NUMBER || t->getType() == TokenType::FRACTION || t->getType() == TokenType::FUNCTION) {
				delete t;
			}
		}
	}
    /*
     * Evaluates a function arguments list, which starts with a left bracket, ends with a right bracket and is separated by commas.
     * 
     * expr - The NEDA expression containing the arguments list
     * varc - The number of user-defined variables
     * vars - An array of all user-defined variables
     * funcc - The number of user-defined functions
     * funcs - An array of all user-defined functions
     * start - Where to start evaluating. This should be the index of the left bracket marking the beginning of the arguments list.
     * end *(out)* - A uint16_t reference which will be set to the index of the right bracket marking the end of the arguments list.
     * 
     * If there is a syntax error in the arguments list, this function will return an empty DynamicArray<Token*>.
     */
    DynamicArray<Token*> evaluateArgs(const DynamicArray<neda::NEDAObj*>& expr, 
            uint16_t varc, const Variable *vars, uint16_t funcc, const UserDefinedFunction *funcs, uint16_t start, uint16_t &end) {
        
        // Args must start with a left bracket
        if(start < expr.length() && expr[start]->getType() != neda::ObjType::L_BRACKET) {
            return DynamicArray<Token*>();
        }
        uint16_t nesting = 0;
        for(end = start; end < expr.length(); end ++) {
            // Increase and decrease the nesting level accordingly and exit when nesting level is 0
            if(expr[end]->getType() == neda::ObjType::L_BRACKET) {
                ++nesting;
            }
            else if(expr[end]->getType() == neda::ObjType::R_BRACKET) {
                --nesting;
                if(!nesting) {
                    break;
                }
            }
        }
		
        if(nesting != 0) {
            // Mismatched brackets
            return DynamicArray<Token*>();
        }
		
        DynamicArray<Token*> args;
        // Increment to skip the first left bracket
        start ++;
        uint16_t argEnd = start;
        while(start != end) {
            // Take care of nested brackets
            uint16_t nesting = 0;
            for(; argEnd < end; ++argEnd) {
                // Increase and decrease nesting accordingly
                if(expr[argEnd]->getType() == neda::ObjType::L_BRACKET) {
                    ++nesting;
                    continue;
                }
                else if(expr[argEnd]->getType() == neda::ObjType::R_BRACKET) {
                    // Arguments can only end by a comma
                    // Thus if nesting ever reaches a level less than zero, there are mismatched parentheses
                    if(!nesting) {
                        freeTokens(args);
                        return DynamicArray<Token*>();
                    }
                    // Decrease nesting since we now know it's nonzero
                    --nesting;
                    continue;
                }
                // Only end arguments when the nesting level is 0
                // This ensures that expressions like f(g(x, y), 0) get parsed properly
                if(nesting == 0 && extractChar(expr[argEnd]) == ',') {
                    break;
                }
            }

            // Isolate the argument's contents
            const DynamicArray<neda::NEDAObj*> argContents = 
                    DynamicArray<neda::NEDAObj*>::createConstRef(expr.begin() + start, expr.begin() + argEnd);
            Token *arg = evaluate(argContents, varc, vars, funcc, funcs);
            // Syntax error
            if(!arg) {
                freeTokens(args);
                return DynamicArray<Token*>();
            }
            args.add(arg);

            // If we ended on a comma (because this argument isn't the last), skip it
            if(extractChar(expr[argEnd]) == ',') {
                ++argEnd;
            }
            start = argEnd;
        }
		return args;
    }

	// Overloaded instance of the other evaluate() for convenience. Works directly on neda::Containers.
	Token* evaluate(const neda::Container *expr, const DynamicArray<Variable> &vars, const DynamicArray<UserDefinedFunction> &funcs) {
		return evaluate(expr->contents, vars.length(), vars.asArray(), funcs.length(), funcs.asArray());
	}
	/*
	 * Evaluates an expression and returns a token result
	 * Returns nullptr on syntax errors
	 * 
	 * Parameters:
	 * expr - a reference to a DynamicArray of neda::NEDAObjs representing an expression
	 * vars - a reference to a DynamicArray of Variables representing all user-defined variables
     * funcs - a reference to a DynamicArray of UserDefinedFunctions representing all user-defined functions
	 */
    Token* evaluate(const DynamicArray<neda::NEDAObj*> &expr, const DynamicArray<Variable> &vars, const DynamicArray<UserDefinedFunction> &funcs) {
        return evaluate(expr, vars.length(), vars.asArray(), funcs.length(), funcs.asArray());
    }
    // Overloaded instance of the other evaluate() for convenience. Works directly on neda::Containers.
    Token* evaluate(const neda::Container *expr, uint16_t varc, const Variable *vars, uint16_t funcc, const UserDefinedFunction *funcs) {
        return evaluate(expr->contents, varc, vars, funcc, funcs);
    }
    // This is a label that was declared in the startup asm and exported
    // Take its address for the stack limit
    extern "C" void *__stack_limit;
    constexpr uint32_t STACK_DANGER_LIMIT = 0x00000050;
    /*
	 * Evaluates an expression and returns a token result
	 * Returns nullptr on syntax errors
	 * 
	 * Parameters:
	 * exprs - a reference to a DynamicArray of neda::NEDAObjs representing an expression
     * varc - the number of user-defined variables
	 * vars - an array containing all user-defined variables
     * funcc - the number of user-defined functions
     * funcs - an array containing all user-defined functions
	 */
	Token* evaluate(const DynamicArray<neda::NEDAObj*> &exprs, uint16_t varc, const Variable *vars, uint16_t funcc, const UserDefinedFunction *funcs) {
		// This function first parses the NEDA expression to convert it into eval tokens
		// It then converts the infix notation to postfix with shunting-yard
		// And finally evaluates it and returns the result

        // First, perform a stack pointer check to make sure we don't overflow when evaluating recursive functions
        if(__current_sp() + STACK_DANGER_LIMIT <= reinterpret_cast<uint32_t>(&__stack_limit)) {
            return nullptr;
        }

		// This dynamic array holds the result of the first stage (basic parsing)
		DynamicArray<Token*> arr;
		uint16_t index = 0;
		// This variable keeps track of whether the last token was an operator
		bool lastTokenOperator = true;
		// Loop over every NEDA object
		// Since many types of NEDA objects require index increments of more than 1, using a while loop is more clear than a for loop
		while (index < exprs.length()) {
			switch (exprs[index]->getType()) {
			// Left bracket
			case neda::ObjType::L_BRACKET:
			{
				// If the last token was not an operator, then it must be an implied multiplication
				if(!lastTokenOperator) {
                    // This looks very dangerous but in reality all methods of Operator are const
					arr.add(const_cast<Operator*>(&OP_MULTIPLY));
				}
				
				// Look for the matching right bracket
				uint16_t nesting = 1;
				uint16_t endIndex = index + 1;
				for(; endIndex < exprs.length(); ++endIndex) {
					// Increase or decrease the nesting level accordingly to find the correct right bracket
					if(exprs[endIndex]->getType() == neda::ObjType::L_BRACKET) {
						++nesting;
					}
					else if(exprs[endIndex]->getType() == neda::ObjType::R_BRACKET) {
						--nesting;
						// If nesting level reaches 0, then we found it
						if(!nesting) {
							break;
						}
					}
				}
				// If nesting is nonzero, there must be mismatched parentheses
				if(nesting) {
					freeTokens(arr);
					return nullptr;
				}
				// Construct a new array of NEDA objects that includes all object inside the brackets (but not the brackets themselves!)
				const DynamicArray<neda::NEDAObj*> inside = 
                        DynamicArray<neda::NEDAObj*>::createConstRef(exprs.begin() + index + 1, exprs.begin() + endIndex);
				// Recursively calculate the content inside
				Token *insideResult = evaluate(inside, varc, vars, funcc, funcs);
				// If syntax error inside bracket, clean up and return null
				if(!insideResult) {
					freeTokens(arr);
					return nullptr;
				}
				// Otherwise, add result to token array
				arr.add(insideResult);
				// Move on to the next part
				index = endIndex + 1;
				// Last token was not an operator
				lastTokenOperator = false;
				break;
			} // neda::ObjType::L_BRACKET

			// Right brackets
			case neda::ObjType::R_BRACKET:
			{
				// Since the processing for left brackets also handle their corresponding right brackets,
				// encountering a right bracket means there are mismatched parentheses. 
				// Clean up and signal error.
				freeTokens(arr);
				return nullptr;
			} // neda::ObjType::R_BRACKET

			// Fractions
			case neda::ObjType::FRACTION:
			{
				// Recursively evaluate the numerator and denominator
				Token *num = evaluate((neda::Container*) ((neda::Fraction*) exprs[index])->numerator, varc, vars, funcc, funcs);
				Token *denom = evaluate((neda::Container*) ((neda::Fraction*) exprs[index])->denominator, varc, vars, funcc, funcs);
				// If one of them results in an error, clean up and return null
				if(!num || !denom) {
					// Since deleting nullptrs are allowed, no need for checking
					delete num;
					delete denom;
					freeTokens(arr);
					return nullptr;
				}
				// Otherwise, call the division operator to evaluate the fraction and add it to the tokens list
                // Temporarily set autoFractions to true so a fraction is created no matter what
                bool tmp = autoFractions;
                autoFractions = true;
				arr.add(OP_DIVIDE(num, denom));
                autoFractions = tmp;

				// Move on to the next object
				++index;
				lastTokenOperator = false;
				break;
			} // neda::ObjType::FRACTION

			// Superscripts (exponentiation)
			case neda::ObjType::SUPERSCRIPT:
			{
                // If the last thing in the tokens array was a matrix, this may be a transpose or inverse operation
                if(arr[arr.length() - 1]->getType() == TokenType::MATRIX) {
                    const auto &c = static_cast<neda::Container*>(static_cast<neda::Superscript*>(exprs[index])->contents)->contents;

                    // Transpose
                    if(c.length() == 1 && extractChar(c[0]) == 'T') {
                        arr.add(const_cast<Operator*>(&OP_TRANSPOSE));
                        // Break here so the rest of the code isn't executed
                        ++index;
                        lastTokenOperator = false;
                        break;
                    }
                    // Inverse
                    else if(c.length() == 2 && extractChar(c[0]) == '-' && extractChar(c[1]) == '1') {
                        arr.add(const_cast<Operator*>(&OP_INVERSE));
                        // Break here so the rest of the code isn't executed
                        ++index;
                        lastTokenOperator = false;
                        break;
                    }
                }
				// Recursively evaluate the exponent
				Token *exponent = evaluate((neda::Container*) ((neda::Superscript*) exprs[index])->contents, varc, vars, funcc, funcs);
				// If an error occurs, clean up and return null
				if(!exponent) {
					freeTokens(arr);
					return nullptr;
				}
				// If the exponent is a matrix, return NaN
				// We really don't want to do the Taylor series of exp(A)
				if(exponent->getType() == TokenType::MATRIX) {
					delete exponent;
					freeTokens(arr);
					return new Number(NAN);
				}
				// Otherwise, turn it into an exponentiation operator and the value of the exponent
				arr.add(const_cast<Operator*>(&OP_EXPONENT));
				arr.add(exponent);
				// Move on to the next token
				++index;
				lastTokenOperator = false;
				break;
			} // neda::ObjType::SUPERSCRIPT

			// Radicals
			case neda::ObjType::RADICAL:
			{   
				// If the last token was not an operator there must be an implied multiplication
				if(!lastTokenOperator) {
					arr.add(const_cast<Operator*>(&OP_MULTIPLY));
				}
				// Used to store the base
				Token *n;
				// If the base exists, recursively evaluate it
				if(((neda::Radical*) exprs[index])->n) {
					n = evaluate((neda::Container*) ((neda::Radical*) exprs[index])->n, varc, vars, funcc, funcs);
				}
				// No base - implied square root
				else {
					n = new Number(2);
				}
				// Recursively evaluate the contents of the radical
				Token *contents = evaluate((neda::Container*) ((neda::Radical*) exprs[index])->contents, varc, vars, funcc, funcs);
				// If an error occurs, clean up and return null
				if(!n || !contents) {
					// nullptr deletion allowed; no need for checking
					delete n;
					delete contents;
					freeTokens(arr);
					return nullptr;
				}
				// Convert the radical into an exponentiation operation
				// Separate processing for numbers vs fractions
				if(n->getType() == TokenType::NUMBER) {
					((Number*) n)->value = 1 / ((Number*) n)->value;
				}
				else if(n->getType() == TokenType::FRACTION) {
					int64_t temp = ((Fraction*) n)->num;
					((Fraction*) n)->num = ((Fraction*) n)->denom;
					((Fraction*) n)->denom = temp;
				}
				// If it's neither a number nor a fraction, then it's a matrix...
				// Don't even bother
				else {
					delete n;
					delete contents;
					freeTokens(arr);
					return new Number(NAN);
				}
				// Evaluate the radical and add the result to the tokens array
				arr.add(OP_EXPONENT(contents, n));
				// Move on to the next object
				++index;
				lastTokenOperator = false;
				break;
			} // neda::ObjType::RADICAL

			// Characters
			case neda::ObjType::CHAR_TYPE:
			{   
				// Get the character
				char ch = extractChar(exprs[index]);
				// If the character is a space, ignore it
				if (ch == ' ') {
					++index;
					break;
				}
				// See if the character is a known operator
				const Operator *op = Operator::fromChar(ch);
				// Check for equality operator which is two characters and not handled by Operator::fromChar
				if((ch == '=' || ch == '!') && index + 1 < exprs.length() && extractChar(exprs[index + 1]) == '=') {
					op = ch == '=' ? &OP_EQUALITY : &OP_NOT_EQUAL;
					++index;
				}
				// Check if the character is an operator
				if (op) {
					// Check for unary operators
					if(lastTokenOperator && (op->type == Operator::Type::PLUS || op->type == Operator::Type::MINUS)) {
						// Allow unary pluses, but don't do anything
						if(op->type == Operator::Type::MINUS) {
							arr.add(const_cast<Operator*>(&OP_NEGATE));
						}
					}
					else {
						// Otherwise add the operator normally
						arr.add(const_cast<Operator*>(op));
					}
                    ++index;
                    lastTokenOperator = true;
                    break;
				}

				// Otherwise, it's probably a number or a variable
				bool isNum;
				// Find its end
				uint16_t end = findTokenEnd(&exprs, index, 1, isNum);
				// Copy over the characters into a string
				char *str = new char[end - index + 1];
				for (uint16_t i = index; i < end; i++) {
					char ch = extractChar(exprs[i]);
					// Convert the x10^x character to a e (parsed by atof)
					if(ch == LCD_CHAR_EE) {
						str[i - index] = 'e';
					}
					else {
						str[i - index] = ch;
					}
				}
				// Add null terminator
				str[end - index] = '\0';

                // Special processing for identity matrix and 0 matrix notation
                if(end < exprs.length() && exprs[end]->getType() == neda::ObjType::SUBSCRIPT && (strcmp(str, "I") == 0 || strcmp(str, "0") == 0)) {
                    // Evaluate the contents of the subscript
                    Token *res = evaluate(static_cast<neda::Container*>(static_cast<neda::Superscript*>(exprs[end])->contents), varc, vars, funcc, funcs);
                    // Check for syntax errors, noninteger result, and out of bounds
                    double n;
                    if(!res || res->getType() == TokenType::MATRIX || (n = extractDouble(res), !isInt(n)) || n <= 0 || n > 255) {
                        delete res;
                        delete[] str;
                        freeTokens(arr);
                        return nullptr;
                    }
                    delete res;

                    Matrix *mat = new Matrix(static_cast<uint8_t>(n), static_cast<uint8_t>(n));
                    // If identity matrix, fill it in
                    if(str[0] == 'I') {
                        for(uint8_t i = 0; i < mat->m; i ++) {
                            mat->setEntry(i, i, 1);
                        }
                    }
                    
                    arr.add(mat);
                    delete[] str;
                    index = end + 1;
                    lastTokenOperator = false;
                    break;
                }

                // Special processing for unit conversions
                if(end < exprs.length() && extractChar(exprs[end]) == LCD_CHAR_RARW) {
                    // Find the other unit
                    index = end + 1;
                    end = findTokenEnd(&exprs, index, 1, isNum);

                    // Copy the other unit
                    char *unit = new char[end - index + 1];
                    for(uint16_t i = index; i < end; i ++) {
                        unit[i - index] = extractChar(exprs[i]);
                    }
                    unit[end - index] = '\0';

                    // Evaluate its arguments
                    auto args = evaluateArgs(exprs, varc, vars, funcc, funcs, end, end);
                    if(args.length() != 1 || args[0]->getType() == TokenType::MATRIX) {
                        // Syntax error
                        freeTokens(args);
                        freeTokens(arr);
                        delete[] unit;
                        delete[] str;
                        return nullptr;
                    }

                    double result = convertUnits(extractDouble(args[0]), str, unit);
                    if(isnan(result)) {
                        // Syntax error
                        freeTokens(args);
                        freeTokens(arr);
                        delete[] unit;
                        delete[] str;
                        return nullptr;
                    }
                    // Add the result
                    arr.add(new Number(result));

                    freeTokens(args);
                    delete[] unit;
                    delete[] str;
                    index = end + 1;
                    lastTokenOperator = false;
                    break;
                }

				const Function *func = nullptr;
				const UserDefinedFunction *uFunc = nullptr;
				// If the token isn't a number
				if(!isNum) {
					// Special processing for logarithms:
					if(strcmp(str, "log") == 0) {
						// See if the next object is a subscript (log base)
						if(end < exprs.length() && exprs[end]->getType() == neda::ObjType::SUBSCRIPT) {
							// If subscript exists, recursively evaluate it
							Token *sub = evaluate((neda::Container*) ((neda::Subscript*) exprs[end])->contents, varc, vars, funcc, funcs);
							// If an error occurs, clean up and return
							if(!sub) {
								freeTokens(arr);
								delete[] str;
								return nullptr;
							}
							// Use the log change of base property to convert it to a base 2 log
							double base = extractDouble(sub);
							delete sub;
							double multiplier = 1 / log2(base);
							// Convert it to a multiplication with a base 2 log
							arr.add(new Number(multiplier));
							// Use special multiply to ensure the order of operations do not mess it up
							arr.add(const_cast<Operator*>(&OP_SP_MULT));
							// The function is base 2 log
							func = new Function(Function::Type::LOG2);
							// Increment end so the index gets set properly afterwards
							++end;
						}
						// Default log base: 10
						else {
							// The function is base 10 log
							func = new Function(Function::Type::LOG10);
						}
						// Move on to evaluate the arguments 
						goto evaluateFunc;
					}
					// If it's not a log, see if it's a built-in function
					else {
						func = Function::fromString(str);
						// If it's not a normal function then try to find a user function that matches
						if(!func) {
							// Loop through all functions
							for(uint16_t i = 0; i < funcc; ++i) {
								// Compare with all the names of user-defined functions
								if(strcmp(funcs[i].name, str) == 0) {
									// If found, set uFunc to point to it
									uFunc = funcs + i;
									break;
								}
							}
						}
						// Add the function if it's valid
						if(func || uFunc) {
evaluateFunc:
							// Implied multiplication
							if(!lastTokenOperator) {
								arr.add(const_cast<Operator*>(&OP_MULTIPLY));
							}
                            auto args = evaluateArgs(exprs, varc, vars, funcc, funcs, end, end);
							// Verify that the number of arguments is correct
							// Make sure to handle user-defined functions as well
							if((func && func->getNumArgs() != args.length()) || (uFunc && uFunc->argc != args.length())) {
								freeTokens(arr);
								freeTokens(args);
								delete[] str;
								delete func;
								return nullptr;
							}
							// Evaluate
							Token *result;
							// Regular function - cast and call to evaluate
							if(func) {
								result = (*func)(args.asArray());
								// If result cannot be computed, syntax error
								if(!result) {
									freeTokens(arr);
									freeTokens(args);
									delete[] str;
									delete func;
									return nullptr;
								}
								delete func;
							}
							// User-defined function
							else {
								// Evaluate a user-defined function by creating a new environment in which the variables list
								// also contain the function arguments. The other variables and functions are also kept so that
								// functions can call other functions. 
                                // The function arguments are first in the list so that they have precedence over other 
                                // variables.
								// However, this doesn't handle recursion! Recursive functions without an exit condition will
                                // cause a stack overflow.

								// Construct a new variables list containing the arguments and normal variables
                                Variable *newVars = new Variable[varc + uFunc->argc];
                                // Copy in the names and values of function arguments
                                for(uint8_t i = 0; i < uFunc->argc; i ++) {
                                    newVars[i].name = uFunc->argn[i];
                                    newVars[i].value = args[i];
                                }
								// Copy in the names and values of variables
								for(uint16_t i = 0; i < varc; i ++) {
                                    newVars[i + uFunc->argc] = vars[i];
								}

								// Evaluate
								result = evaluate(uFunc->expr, varc + uFunc->argc, newVars, funcc, funcs);
								// Syntax error, cleanup
								if(!result) {
                                    delete[] newVars;

									freeTokens(arr);
									freeTokens(args);
									delete[] str;
									return nullptr;
								}

								// Cleanup
								delete[] newVars;
							}

							// Free args
							freeTokens(args);
							// Add result
							arr.add(result);
							lastTokenOperator = false;
                            // Increment end to skip the ending right bracket
							++end;
						}
						// If not a function, check if it's a constant or a variable
						else {
							// Implied multiplication
							if(!lastTokenOperator && (arr.length() != 0 && arr[arr.length() - 1] != &OP_MULTIPLY)) {
								arr.add(const_cast<Operator*>(&OP_MULTIPLY));
							}
							// If n is nonnull it must be added, so no need for cleanup for this dynamically allocated variable
							Number *n = Number::constFromString(str);
							// Add if it's a valid constant
							if(n) {
								arr.add(n);
							}
							// Otherwise check if it's a valid variable
							else {
								// Loop through all variables
								uint16_t i;
								for(i = 0; i < varc; i ++) {
									// Compare with each variable name
									if(strcmp(str, vars[i].name) == 0) {
										// We found a match!
										if(vars[i].value->getType() == TokenType::NUMBER) {
											arr.add(new Number(((Number*) vars[i].value)->value));
										}
										else if(vars[i].value->getType() == TokenType::FRACTION) {
											arr.add(new Fraction(((Fraction*) vars[i].value)->num, ((Fraction*) vars[i].value)->denom));
										}
										// Matrices
										else {
											arr.add(new Matrix(*((Matrix*) vars[i].value)));
										}
										break;
									}
								}
								// If no match was found, cleanup and return
								if(i == varc) {
									freeTokens(arr);
									delete[] str;
									return nullptr;
								}
							}
							lastTokenOperator = false;
						}
					}
				}
				// If it's a number, parse it with atof and add its value
				else {
					arr.add(new Number(atof(str)));
					index = end;
					lastTokenOperator = false;
				}
				// Clean up the string buffer and move on
				delete[] str;
				index = end;
				break;

			} // neda::ObjType::CHAR_TYPE

			// Summation (sigma) or product (pi)
			case neda::ObjType::SIGMA_PI:
			{
				// First recursively evaluate the end value
				Token *end = evaluate((neda::Container*) ((neda::SigmaPi*) exprs[index])->finish, varc, vars, funcc, funcs);
				if(!end) {
					freeTokens(arr);
					return nullptr;
				}
				// Evaluate the starting value
				// Split the starting condition at the equals sign
				auto &startContents = ((neda::Container*) ((neda::SigmaPi*) exprs[index])->start)->contents;
				uint16_t equalsIndex = findEquals(startContents, true);
				// If equals sign not found, syntax error
				if(equalsIndex == 0xFFFF) {
					delete end;
					freeTokens(arr);
					return nullptr;
				}
				// Attempt to evaluate the starting condition assign value
				const DynamicArray<neda::NEDAObj*> startVal = 
                        DynamicArray<neda::NEDAObj*>::createConstRef(startContents.begin() + equalsIndex + 1, startContents.end());
				Token *start = evaluate(startVal, varc, vars, funcc, funcs);
				// Check for syntax error
				if(!start) {
					delete end;
					freeTokens(arr);
					return nullptr;
				}
				// Matrices are not allowed as counters
				if(start->getType() == TokenType::MATRIX) {
					delete end;
					delete start;
					freeTokens(arr);
					return nullptr;
				}
				// Isolate the variable name
				char *vName = new char[equalsIndex + 1];
				// Extract each character
				for(uint16_t i = 0; i < equalsIndex; i ++) {
					vName[i] = extractChar(startContents[i]);
				}
				// Null termination
				vName[equalsIndex] = '\0';

                // Just like in the case of a user-defined function, we create a new environment with the counter
                // variable. The counter variable is copied in first and gets precedence over other variables.
				// Construct new variable arrays
                Variable *newVars = new Variable[varc + 1];
				// Copy in the counter variable
                newVars[0].name = vName;
                newVars[0].value = start;
				// Copy existing variables
				for(uint16_t i = 0; i < varc; i ++) {
                    newVars[i + 1] = vars[i];
				}

				// Find the type of operation by extracting the symbol
				auto &type = ((neda::SigmaPi*) exprs[index])->symbol;
				// The accumulated value
				Token *val = nullptr;
				// While the start is still less than or equal to the end
				while(compareTokens(start, end) <= 0) {
					// Evaluate the inside expression
					Token *n = evaluate((neda::Container*) ((neda::SigmaPi*) exprs[index])->contents, varc + 1, newVars, funcc, funcs);

					// If there is ever a syntax error then cleanup and exit
					if(!n) {
						delete end;
						delete start;
						delete[] vName;
						delete[] newVars;
						delete val;
						freeTokens(arr);
						return nullptr;
					}
					// Add or multiply the expressions if val exists
					// Operate takes care of deletion of operands
					if(val) {
						val = (type.data == lcd::CHAR_SUMMATION.data ? OP_PLUS : OP_MULTIPLY)(val, n);
					}
					// Set val if it doesn't exist
					else {
						val = n;
					}
					// Add one to the counter variable
					if(start->getType() == TokenType::NUMBER) {
						++((Number*) start)->value;
					}
					// Since counter cannot be a matrix, if it's not a number then it must be a fraction
					else {
						((Fraction*) start)->num += ((Fraction*) start)->denom;
					}
				}
				// If val was not set, then there were no iterations
				// Set it to a default value instead
				// For summation this is 0, for product it is 1
				if(!val) {
					val = new Number(type.data == lcd::CHAR_SUMMATION.data ? 0 : 1);
				}

				// Insert the value
				arr.add(val);
				// Cleanup
				delete end;
				delete start;
				delete[] vName;
				delete[] newVars;
				// Move on to the next object
				++index;
				lastTokenOperator = false;
				break;
			} // neda::ObjType::SIGMA_PI

			// Matrices
			case neda::ObjType::MATRIX:
			{
				// Implied multiplication
				if(!lastTokenOperator) {
					arr.add(const_cast<Operator*>(&OP_MULTIPLY));
				}
				neda::Matrix *nMat = static_cast<neda::Matrix*>(exprs[index]);
				// Convert to a eval::Matrix
				Matrix *mat = new Matrix(nMat->m, nMat->n);
                // Matrices can be constructed from a bunch of column vectors too
                bool fromVecs = false;
				// Evaluate every entry
				for(uint16_t i = 0; i < nMat->m * nMat->n; i ++) {
					Token *n = evaluate((neda::Container*) nMat->contents[i], varc, vars, funcc, funcs);
					// Check for syntax error
					if(!n) {
						delete mat;
						freeTokens(arr);
						return nullptr;
					}
                    if(!fromVecs) {
                        // Matrices can't be inside matrices...
                        if(n->getType() == TokenType::MATRIX) {
                            // Unless the matrix inside is actually a column vector
                            // And we're on the first entry
                            // And the neda::Matrix only has one row
                            // In which case the matrix would be constructed using column vectors
                            if(i == 0 && static_cast<Matrix*>(n)->n == 1 && nMat->m == 1) {
                                fromVecs = true;
                                // Reconstruct the eval::Matrix
                                delete mat;
                                mat = new Matrix(static_cast<Matrix*>(n)->m, nMat->n);
                                goto constructMatrixFromVectors;
                            }
                            delete mat;
                            delete n;
                            freeTokens(arr);
                            return nullptr;
                        }
                        // Ignore fractions and just use their numerical values
                        mat->contents[i] = extractDouble(n);
                    }
                    else {
                        // Check that the vector only has 1 column and the rows are as expected
                        if(n->getType() != TokenType::MATRIX || static_cast<Matrix*>(n)->n != 1
                                || static_cast<Matrix*>(n)->m != mat->m) {
                            delete mat;
                            delete n;
                            freeTokens(arr);
                            return nullptr;
                        }
constructMatrixFromVectors:
                        // Fill in the column of the matrix with the entires in this column vector
                        for(uint8_t row = 0; row < mat->m; row ++) {
                            mat->setEntry(row, i, static_cast<Matrix*>(n)->contents[row]);
                        }
                    }
                    delete n;
				}
				// Insert value
				arr.add(mat);
				// Move on to the next object
				++index;
				lastTokenOperator = false;
				break;
			} // neda::ObjType::MATRIX

            // Piecewise functions
            case neda::ObjType::PIECEWISE:
            {
                // Implied multiplication
				if(!lastTokenOperator) {
					arr.add(const_cast<Operator*>(&OP_MULTIPLY));
				}
                neda::Piecewise *p = static_cast<neda::Piecewise*>(exprs[index]);
                
                Token *val = nullptr;
                for(uint8_t i = 0; i < p->pieces; i ++) {

                    // Evaluate the condition
                    Token *n = evaluate(static_cast<neda::Container*>(p->conditions[i]), varc, vars, funcc, funcs);
                    bool isElse = false;
                    // Syntax error
                    if(!n) {
                        // See if the condition is just "else"
                        // Of course, this doesn't work if this is the first piece
                        neda::Container *condition = static_cast<neda::Container*>(p->conditions[i]);
                        if(i != 0 && condition->contents.length() == 4 
                                && extractChar(condition->contents[0]) == 'e' && extractChar(condition->contents[1]) == 'l'
                                && extractChar(condition->contents[2]) == 's' && extractChar(condition->contents[3]) == 'e') {
                            isElse = true;
                        }
                        else {
                            freeTokens(arr);
                            return nullptr;
                        }
                    }
                    // If it's an else clause condition is directly set to true
                    int8_t condition = isElse ? 1 : isTruthy(n);
                    delete n;
                    // Condition undefined
                    // Then the entire expression is undefined
                    if(condition == -1) {
                        freeTokens(arr);
                        return new Number(NAN);
                    }
                    // Condition is true
                    else if(condition == 1) {
                        // Evaluate value
                        val = evaluate(static_cast<neda::Container*>(p->values[i]), varc, vars, funcc, funcs);

                        // Syntax error
                        if(!val) {
                            freeTokens(arr);
                            return nullptr;
                        }
                        break;
                    }
                    // If condition is false, move on
                }
                // No condition was true - value is undefined
                if(!val) {
                    freeTokens(arr);
                    return new Number(NAN);
                }

                arr.add(val);
                ++index;
                lastTokenOperator = false;
                break;
            } // neda::ObjType::PIECEWISE

            // Subscripts
            case neda::ObjType::SUBSCRIPT:
            {
                // Since if the subscript was part of a log expression, it would already be handled by neda::ObjType::CHAR_TYPE,
                // currently the only use for the subscript is for matrix indices
                // Check the last item in the array and make sure it's a matrix
                if(arr[arr.length() - 1]->getType() != TokenType::MATRIX) {
                    freeTokens(arr);
                    return nullptr;
                }
                auto &contents = static_cast<neda::Container*>(static_cast<neda::Subscript*>(exprs[index])->contents)->contents;
                // See if there's a comma in the expression
                uint16_t commaIndex;
                // Find the comma
                // Take care of brackets
                uint16_t nesting = 0;
                for(commaIndex = 0; commaIndex < contents.length(); commaIndex ++) {

                    if(contents[commaIndex]->getType() == neda::ObjType::L_BRACKET) {
                        nesting ++;
                    }
                    else if(contents[commaIndex]->getType() == neda::ObjType::R_BRACKET) {
                        if(nesting > 0) {
                            nesting --;
                        }
                        else {
                            freeTokens(arr);
                            return nullptr;
                        }
                    }
                    else if(nesting == 0 && extractChar(contents[commaIndex]) == ',') {
                        break;
                    }
                }

                // Get the matrix
                const Matrix *mat = static_cast<const Matrix*>(arr[arr.length() - 1]);

                Token *result = nullptr;
                
                // No comma
                if(commaIndex == contents.length()) {
                    Token *t = evaluate(contents, varc, vars, funcc, funcs);
                    // Check for syntax errors in expression, or noninteger result
                    if(!t || t->getType() == TokenType::MATRIX || !isInt(extractDouble(t))) {
                        delete t;
                        freeTokens(arr);
                        return nullptr;
                    }

                    double d = extractDouble(t);
                    if(!canCastProperly<double, uint8_t>(d - 1)) {
                        freeTokens(arr);
                        delete t;
                        return nullptr;
                    }
                    uint8_t index = static_cast<uint8_t>(d - 1);
                    delete t;

                    // For vectors, just take the number
                    if(mat->n == 1) {
                        if(index < mat->m) {
                            result = new Number((*mat)[index]);
                        }
                        else {
                            freeTokens(arr);
                            return nullptr;
                        }
                    }
                    else {
                        // Otherwise take a row vector
                        result = mat->getRowVector(index);
                        if(!result) {
                            freeTokens(arr);
                            return nullptr;
                        }
                    }
                }
                else {
                    const DynamicArray<neda::NEDAObj*> rowExpr = 
                            DynamicArray<neda::NEDAObj*>::createConstRef(contents.begin(), contents.begin() + commaIndex);
                    const DynamicArray<neda::NEDAObj*> colExpr = 
                            DynamicArray<neda::NEDAObj*>::createConstRef(contents.begin() + commaIndex + 1, contents.end());
                    
                    Token *row = evaluate(rowExpr, varc, vars, funcc, funcs);
                    // Check for syntax errors in expression, or noninteger result
                    if(!row || row->getType() == TokenType::MATRIX || !isInt(extractDouble(row))) {
                        // Wildcard syntax
                        if(rowExpr.length() == 1 && extractChar(rowExpr[0]) == '*') {
                            row = nullptr;
                        }
                        else {
                            delete row;
                            freeTokens(arr);
                            return nullptr;
                        }
                    }
                    Token *col = evaluate(colExpr, varc, vars, funcc, funcs);
                    if(!col || col->getType() == TokenType::MATRIX || !isInt(extractDouble(col))) {
                        // Wildcard syntax
                        if(colExpr.length() == 1 && extractChar(colExpr[0]) == '*') {
                            col = nullptr;
                        }
                        else {
                            delete row;
                            delete col;
                            freeTokens(arr);
                            return nullptr;
                        }
                    }
                    
                    // Wildcard on column
                    // Take row vector
                    if(row && !col) {
                        // Verify cast into uint8_t
                        double drow = extractDouble(row);
                        if(!canCastProperly<double, uint8_t>(drow - 1)) {
                            delete row;
                            freeTokens(arr);
                            return nullptr;
                        }
                        
                        uint8_t rowInt = static_cast<uint8_t>(drow - 1);
                        result = mat->getRowVector(rowInt);
                        // If out of range, syntax error
                        if(!result) {
                            delete row;
                            freeTokens(arr);
                            return nullptr;
                        }
                    }
                    // Wildcard on row
                    // Take column vector
                    else if(col && !row) {
                        double dcol = extractDouble(col);
                        if(!canCastProperly<double, uint8_t>(dcol - 1)) {
                            delete col;
                            freeTokens(arr);
                            return nullptr;
                        }

                        uint8_t colInt = static_cast<uint8_t>(dcol - 1);
                        result = mat->getColVector(colInt);
                        if(!result) {
                            delete col;
                            freeTokens(arr);
                            return nullptr;
                        }
                    }
                    // Wildcard on both indices
                    // Just copy the matrix itself
                    else if(!col && !row) {
                        result = new Matrix(*mat);
                    }
                    else {
                        double drow = extractDouble(row);
                        double dcol = extractDouble(col);
                        // Verify that the indices can be properly casted into uint8_ts
                        if(!canCastProperly<double, uint8_t>(drow - 1) || !canCastProperly<double, uint8_t>(dcol - 1)) {
                            delete row;
                            delete col;
                            freeTokens(arr);
                            return nullptr;
                        }

                        uint8_t rowInt = static_cast<uint8_t>(drow - 1);
                        uint8_t colInt = static_cast<uint8_t>(dcol - 1);
                        if(rowInt >= mat->m || colInt >= mat->n) {
                            delete row;
                            delete col;
                            freeTokens(arr);
                            return nullptr;
                        }
                        else {
                            result = new Number(mat->getEntry(rowInt, colInt));
                        }
                    }
                    delete row;
                    delete col;
                }

                // Delete the matrix
                delete mat;
                // Replace it with the result
                arr[arr.length() - 1] = result;

                lastTokenOperator = false;
                index ++;
                break;
            } // neda::ObjType::SUBSCRIPT

            // Absolute value
            case neda::ObjType::ABS:
            {
                // Implied multiplication
				if(!lastTokenOperator) {
					arr.add(const_cast<Operator*>(&OP_MULTIPLY));
				}
                Token *t = evaluate(static_cast<neda::Container*>(static_cast<neda::Abs*>(exprs[index])->contents), varc, vars, funcc, funcs);

                if(!t) {
                    freeTokens(arr);
                    return nullptr;
                }

                if(t->getType() == TokenType::NUMBER) {
                    static_cast<Number*>(t)->value = abs(static_cast<Number*>(t)->value);
                    arr.add(t);
                }
                else if(t->getType() == TokenType::FRACTION) {
                    static_cast<Fraction*>(t)->num = abs(static_cast<Fraction*>(t)->num);
                    arr.add(t);
                }
                else {
                    arr.add(new Number(static_cast<Matrix*>(t)->len()));
                    delete t;
                }

                lastTokenOperator = false;
                index ++;
                break;
            } // neda::ObjType::ABS

			default: ++index; break;
			}
		}

		// After that, we should be left with an expression with nothing but numbers, fractions and basic operators
		// Use shunting yard
		Deque<Token*> output(arr.length());
		Deque<Token*> stack;
		for(Token *t : arr) {
			// If token is a number, fraction or matrix, put it in the queue
			if(t->getType() == TokenType::NUMBER || t->getType() == TokenType::FRACTION || t->getType() == TokenType::MATRIX) {
				output.enqueue(t);
			}
			else {
				// Operator
				// Pop all items on the stack that have higher precedence and put into the output queue
				while(!stack.isEmpty() && static_cast<const Operator*>(stack.peek())->getPrecedence() <= static_cast<const Operator*>(t)->getPrecedence()) {
					output.enqueue(stack.pop());
				}
				// Push the operator
				stack.push(t);
			}
		}
		// Transfer all the contents of the stack to the queue
		while(!stack.isEmpty()) {
			output.enqueue(stack.pop());
		}

		// Evaluate
		// Reuse stack
		while(!output.isEmpty()) {
			// Read a token
			Token *t = output.dequeue();
			// If token is a number, fraction or matrix, push onto sta
			if(t->getType() == TokenType::NUMBER || t->getType() == TokenType::FRACTION || t->getType() == TokenType::MATRIX) {
				stack.push(t);
			}
			// Operator
			else {
                // Unary operator
                if(static_cast<const Operator*>(t)->isUnary()) {
                    // If there aren't enough operators, syntax error
                    if(stack.length() < 1) {
                        freeTokens(output);
                        freeTokens(stack);
                        return nullptr;
                    }
                    // Pop the operand
                    Token *operand = stack.pop();
                    // Operate and push
                    Token *result = (*static_cast<const Operator*>(t))(operand);
                    if(result) {
                        stack.push(result);
                    }
                    else {
                        freeTokens(output);
                        freeTokens(stack);
                        return nullptr;
                    }
                }
                else {
                    // If there aren't enough operators, syntax error
                    if(stack.length() < 2) {
                        freeTokens(output);
                        freeTokens(stack);
                        return nullptr;
                    }
                    // Pop the left and right hand side operands
                    Token *rhs = stack.pop();
                    Token *lhs = stack.pop();
                    // Operate and push
                    Token *result = (*static_cast<const Operator*>(t))(lhs, rhs);
                    if(result) {
                        stack.push(result);
                    }
                    else {
                        freeTokens(output);
                        freeTokens(stack);
                        return nullptr;
                    }
                }
			}
		}

		if(stack.length() != 1) {
			// Syntax error: Too many numbers??
			while(!stack.isEmpty()) {
				delete stack.pop();
			}
			return nullptr;
		}
		return stack.pop();
	}
}
