#include "bigint.h"
#include <iostream>
#include <string>
#include <climits>
#include <cmath>
using namespace std;

/*
//   Create a default BigInt with base 10.
*/
BigInt::BigInt(){

    base = 10;
    isPositive = true;

}

/*
//  Create a BigInt with a specified base.
*/
BigInt::BigInt(int setbase){
    if(setbase < 2 || setbase > 36){
        throw InvalidBaseException();
    }
    
    base = setbase;
    isPositive = true;

}

/*
//  Create a BigInt from int (base 10) with a specified base.
//  Here you would convert an int (base 10) into a specified base.
//  e.g., (10,2) <-- 10 base 10 to base 2 --> result:  BigInt with value 1010 base 2
//  e.g., (100,16) <--- 100 base 10 to base 16 --> result: BigInt with value 64 base 16
*/
BigInt::BigInt(int input,int setbase){
    if(setbase < 2 || setbase > 36){
        throw InvalidBaseException();
    }
  
    base = setbase;
    
    // setting sign for input (positive or negative)
    if (input > 0) {
        isPositive = true;
    }
    else if (input < 0) {
        isPositive = false;
        input *= -1;
    }
    else if (input == 0) {
        isPositive = true;
        vec.push_back(0);
    }

    int remainder; //carry = 0;
    while (input > 0) {
        remainder = input % base;
        input /= base;
        if (remainder < 10) {
            vec.push_back(remainder);
        }
        else if (remainder >= 10) {
            vec.push_back(remainder);
        }             
    }
  
}

/*
//  Create a BigInt from string with a specified base.
//  You can assume that the string number input is in the same
//      base as the setbase input.
*/
BigInt::BigInt(const string &s, int setbase){
    if(setbase < 2 || setbase > 36){
        throw InvalidBaseException();
    }
    int lastDigit = 0;
    if (s[0] == '-') {
        isPositive = false;
        lastDigit = 1;
    }
    else {
        isPositive = true;
    }

    int deciNum, temp1, temp2;
    int charToNum;
    base = setbase;
    int hold = 0; // acts like "carrying" a number when adding

    for (int i = s.size() - 1; i >= lastDigit; i--)
    {
        deciNum = ((int)(s[i]) - '0');
        charToNum = ((int)(s[i]) - '0' - 7);
        // if character at s[i] is a number (0 - 9), 
        // then push_back the decimal version of the char
        if (deciNum < 10) {
            // if there is nothing that was "carried over" from past arithmetic
            if (hold == 0) {
                vec.push_back(deciNum);
            }
            // if there is something "carried over"
            else if (hold != 0) {
                deciNum += hold; // add what was carried to the new value
                if (deciNum < 10) {                                             
                    vec.push_back(deciNum);
                }
                else {
                    temp1 = deciNum / base;
                    temp2 = deciNum % base;
                    vec.push_back(temp2);
                    hold = temp1;
/* if (hold != 0) {
    vec.push_back(hold);
} */ 
                }
            }
        }
        // else if the character at s[i] is 10 or a letter (A-Z)
        else {
            // if the character corresponds to an integer 
            // greater than 10, separate the integer into 2 parts 
            // and push_back both numbers
            temp1 = charToNum / base;
            temp2 = charToNum % base;
            if (hold != 0) {
                temp2 += hold;
            }
            vec.push_back(temp2);
            hold = temp1;
        }
    }

    if (hold != 0) {
        vec.push_back(hold);
    }
    
}

/*
//  Create a BigInt from another BigInt.
//  This is a copy constructor.
//  Simply use the assignment operator.
*/
BigInt::BigInt(const BigInt &b){

    *this = b;
    
    /*
    base = b.base;
    isPositive = b.isPositive;;
    for (unsigned int i = 0; i < b.vec.size(); i++) {
        vec.push_back(b.vec[i]);
    }*/

}

/*
// Destructure. You do not have to add anything.
// This function is complete
*/
BigInt::~BigInt(){}



/*
// Convert to string and return the string
//  Display it in its corresponding base
*/
string BigInt::to_string(){

    string bigIntString = "";
    
    if (isPositive == false) {
        bigIntString += "-";
    }

    for (int i = vec.size() - 1; i >= 0; i--)
    {
        if (vec[i] < 10) {
            bigIntString += (char)(vec[i] + '0');
        }
        else if (vec[i] >= 10) {
            bigIntString += (char)(vec[i] + 55);
        }
    }

    return bigIntString;//for now
}

/*
//  Convert BigInt to integer base 10 and return that int
//    If BigInt > INT_MAX, return INT_MAX.
//    If BigInt < INT_MIN, return INT_MIN.
*/
int BigInt::to_int() const{
    int count = 0;
    long long x = 0;
    
    //start at the beginning of vec
    if (isPositive) {
        for (unsigned int i = 0; i < vec.size(); i++) {
            x += (pow(base, count)) * vec[i];
            count++;
        }
    }
    else if (!isPositive) {
        for (unsigned int i = 0; i < vec.size(); i++) {
            x -= (pow(base, count)) * vec[i];
            count++;
        } 
    }
    
    if (x > INT_MAX) {
        return INT_MAX;
    }
    else if (x < INT_MIN) {
        return INT_MIN;
    }
    
    return x;
}

/*
//  Compare a and b.
//     If a = b, return 0.
//     If a < b, return -1.
//     If a > b, return 1.
*/
int BigInt::compare(const BigInt &b) const{
    if(base != b.base){
        throw DiffBaseException();
    }

    // if one of them is positive and the other is negative, 
    // return that the positive BigInt is greater
    if (isPositive && b.isPositive == false) {
        return 1;
    }
    else if (!isPositive && b.isPositive == true) {
        return -1;
    }
    
    if (vec.size() < b.vec.size()) {
        if (isPositive && b.isPositive == true) {
            return -1;
        }
        else if (!isPositive && b.isPositive == false) {
            return 1;
        }
    }
    else if (vec.size() > b.vec.size()) {
        if (isPositive && b.isPositive == true) {
            return 1;
        }
        else if (!isPositive && b.isPositive == false) {
            return -1;
        }
    }
    else if (vec.size() == b.vec.size()) {
        for (int i = vec.size() - 1; i >= 0; i--) {
if (((vec[i] < b.vec[i]) && (isPositive && b.isPositive == true)) || 
((vec[i] > b.vec[i]) && (!isPositive && b.isPositive == false))) {
    return -1;
}
else if (((vec[i] < b.vec[i]) && (!isPositive && b.isPositive == false)) || 
((vec[i] > b.vec[i]) && (isPositive && b.isPositive == true))) {
    return 1;
}
        }
    }
  
  return 0;//for now
}



/*
//  Assignment operator. (i.e., BigInt a = b;)
*/
const BigInt & BigInt::operator = (const BigInt &b){

    
    base = b.base;
    isPositive = b.isPositive;
    vec = b.vec;
    
    
    /*if (*this == b) {
        return *this;
    }
    
    if (b.isPositive) {
        isPositive = true;
    }
    else {
        isPositive = false;
    }
        
    vec.clear();
    for (int i = 0; i < (int)b.vec.size(); i++) {
        vec.push_back(b.vec[i]);
    }*/
  
  return *this;
}


/*
//  Addition assignment operator.
//    - Compute a = a + b.
*/
const BigInt & BigInt::operator += (const BigInt &b){
    if(base != b.base){
        throw DiffBaseException();
    }

    //create a zero bigInt for use throughout the function
    BigInt zero(0, base);
    
    if (isPositive != b.isPositive) {
        //a is positive, b is negative
        if (isPositive) {
            //change sign of b to positive with a copy
            BigInt copyB(b);
            copyB.isPositive = true;
            //do a-=b
            *this -= copyB;
        }
        //a is negative, b is positive
        else {
            //change sign of a to positive, call subtraction, flip sign at end
            isPositive = true;
            *this -= b;
            if(vec[0] == 0 && vec.size() == 1) {
                isPositive = true;
            }
            else {
                isPositive = !isPositive;

            }
        }
        return *this;
    }
    else if (isPositive == b.isPositive) {
        unsigned int largerSize = max(vec.size(), b.vec.size());
        int carry = 0, sum = 0, numA = 0, numB = 0;
        for (unsigned int i=0;i<largerSize;i++) {
            if (i < vec.size()) {
                numA = vec[i];
            }
            else {
                numA = 0;
            }
            if (i < b.vec.size()) {
                numB = b.vec[i];
            }
            else {
                numB = 0;
            }
            sum = numA + numB + carry;
            if (sum >= base) {
                carry = sum / base;
            }else {
                carry = 0;
            }
            if (i < vec.size()) {
                vec[i] = sum%base;
            }
            else {
                vec.push_back(sum%base);
            }
        }
        if (carry != 0) {
            vec.push_back(carry);
        }   
        return *this;
    }
    
    return *this;
}


/*
//  Subtraction assignment operator.
//    - Compute a = a - b
*/
const BigInt & BigInt::operator -= (const BigInt &b){
    if(base != b.base){
        throw DiffBaseException();
    }
    
    BigInt zero(0, base);
    
    bool aIsGreater;
    if (*this < b) {
        aIsGreater = false;
    }
    else {
        aIsGreater = true;
    }
    
    //if a and b have different signs
    if (isPositive != b.isPositive) {
        // if a is positive and b is negative
        if (isPositive) {
            //a -= (-b)  -----> a+=b
            BigInt copyB(b);
            copyB.isPositive = true;
            *this += copyB;
        }
        //if a is negative and b is positive
        else {
            //-a -= b ----> -(a+b)
            isPositive = true;
            *this += b;
            isPositive = false;
        }
    }
    else if (isPositive == b.isPositive) {
        unsigned int largerSize = max(vec.size(), b.vec.size());
        int borrow = 0, sum = 0, numA = 0, numB = 0;
        //subtract by 0
        if (b == zero) {
            return *this;
        }
        // subtract by self
        if (*this == b) {
            *this = zero;
            return *this;
        }
        for (unsigned int i=0;i<largerSize;i++) {
            if (i < vec.size()) {
                numA = vec[i];
            }
            else {
                numA = 0;
            }
            if (i < b.vec.size()) {
                numB = b.vec[i];
            }
            else {
                numB = 0;
            }
            if (isPositive) {
                if (aIsGreater) {
                    sum = numA - numB - borrow;                }
                else {
                    sum = numB - numA - borrow;
                }
            }
            else {
                if (aIsGreater) {
                    sum = numB - numA - borrow;
                }
                else {
                    sum = numA - numB - borrow;
                }
            }
            if (sum < 0) {
                sum += base;
                borrow = 1;
            }
            else {
                borrow = 0;
            }
            if (i < vec.size()) {
                vec[i] = sum;
            }
            else {
                vec.push_back(sum);
            }
        }
        if (!aIsGreater && isPositive) {
            isPositive = !isPositive;
        }
        else if (aIsGreater && !isPositive) {
            isPositive = !isPositive;
        }
    }

    for (unsigned int i = vec.size() - 1; i > 0; i--) {
        if (vec[i] == 0) {
            vec.pop_back();
        }
        else {
            break;
        }
    }
    return *this;

}                                                                        


/*
//  Multiplication assignment operator.
//    - Compute a = a * b;
*/
const BigInt & BigInt::operator *= (const BigInt &b){
    if(base != b.base){
        throw DiffBaseException();
    }

    BigInt zero(0, base);
    BigInt one(1, base);
    
    //if either is 0, return 0
    if (*this == zero) {
        return *this;
    }
    if (b == zero) {
        *this = b;
        return *this;
    }
    
    // set final sign of result
    bool finalSign;
    if (isPositive != b.isPositive) {
        finalSign = false;
    }
    else {
        finalSign = true;
    }
    
    //if either is one, return the other with appropriate sign
    if (b == one) {
        isPositive = finalSign;
        return *this;
    }
    else if (*this == one) {
        vec = b.vec;
        isPositive = finalSign;
        return *this;
    }
    
    BigInt tempSum(0, base);
    int carry = 0, product = 0;
    for (size_t i = 0; i<b.vec.size();i++) {
        string buffer(i, '0'); 
        // or string buffer = ""
        // for (int z = 0; z<i;z++) {buffer+= '0' }
        BigInt temp(buffer, base);
        for (size_t j = 0;j<vec.size();j++) {
            product = (vec[j] * b.vec[i]) + carry;
            carry = product / base;
            temp.vec.push_back(product%base);
        }
        if (carry != 0) {
            temp.vec.push_back(carry);
        }
        tempSum += temp;
        carry = 0;
    }
    
    *this = tempSum;
    isPositive = finalSign;
    
    return *this;
}

/*
//  Division assignment operator.
//    - Call 'divisionMain' to compute a = a / b;
*/
const BigInt & BigInt::operator /= (const BigInt &b){
    if(base != b.base){
        throw DiffBaseException();
    }
    if(b.vec.size() == 1 && b.vec[0] == 0){
        throw DivByZeroException();
    }
    /************* You complete *************/
    BigInt quotient(base);
    BigInt remainder(base);
    
    divisionMain(b, quotient, remainder);
    
    *this = quotient;
    
    return *this;

}

/*
//  Modulus assignment operator.
//    - Call 'divisionMain' to compute a = a % b.
//    - Note: remainder takes the sign of the dividend.
*/
const BigInt & BigInt::operator %= (const BigInt &b){
    if(base != b.base){
        throw DiffBaseException();
    }
    if(b.vec.size() == 1 && b.vec[0] == 0){
        throw DivByZeroException();
    }
    /************* You complete *************/
    BigInt quotient(base);
    BigInt remainder(base);
    
    divisionMain(b, quotient, remainder);
    
    *this = remainder;
    
    return *this;

}

/*
//  Main function for the Division and Modulus operator.
//     - Compute (q)uotient and (r)emainder: a = (b * q) + r.
//     - Note: NO fraction. This should behave like integer division
//             where 2/10 = 0, not 0.2
*/
void BigInt::divisionMain(const BigInt &b, BigInt &quotient, BigInt &remainder){

    /************* You complete *************/

    bool finalSign;
    if (isPositive != b.isPositive) {
        finalSign = false;
    }
    else {
        finalSign = true;
    }
    
    bool startSignOfA = isPositive;
    isPositive = true;
    
    BigInt zero(0, base);
    BigInt one(1, base);
    BigInt negOne(-1, base);
    
    if (b.vec.size() == 1 && b.vec[0] == 1) {
        quotient = *this;
        quotient.isPositive = finalSign;
        remainder = zero;
        return;
    }
    
    BigInt copyofB(b);
    copyofB.isPositive = true;
    
    if (*this == copyofB) {
        quotient = one;
        quotient.isPositive = finalSign;
        remainder = zero;
        return;
    }
    
    if (*this == zero) {
        quotient = zero;
        remainder = zero;
        remainder.isPositive = startSignOfA;
        return;
    }
    
    if (*this < copyofB) {
        quotient = zero;
        remainder = *this;
        remainder.isPositive = startSignOfA;
        return;
    }
    
    int position = vec.size() - 1;
    BigInt seq(vec[position], base);
    while (seq < copyofB) {
        position -= 1;
        seq.vec.insert(seq.vec.begin(), vec[position]);
    }
    while (position >= 0) {
        int count = 0;
        while (seq >= copyofB) {
            seq -= copyofB;
            count += 1;
        }
        quotient.vec.insert(quotient.vec.begin(), count);
        if (seq.vec.size() == 1 && seq.vec[0] == 0) {
            seq.vec.pop_back(); // or seq.vec.clear()
        }
        if (position - 1 >= 0) {
            seq.vec.insert(seq.vec.begin(), vec[position -1]);
        }
        position -= 1;
    }
    
    if (seq.vec.empty()) {
        remainder.vec.push_back(0);
        remainder.isPositive = true;
    }
    else {
        remainder = seq;
        remainder.isPositive = startSignOfA;
    }
    
    quotient.isPositive = finalSign;
}


/*
//  Exponentiation assignment function.
//     - Compute a BigInt whose value is a = pow(a,b).
*/
const BigInt & BigInt::exponentiation(const BigInt &b){
    if(base != b.base){
        throw DiffBaseException();
    }
    if(!b.isPositive){
        throw ExpByNegativeException();
    }
    /************* You complete *************/
    BigInt result(1, base), exp = b, zero(0, base), two(2, base);
    BigInt one (1, base);
    
    while (exp > zero) {
        if ((exp % two) == zero) {
            *this *= *this;
            exp/=two;
        }
        else {
            exp-=one;
            result *= *this;            
        }
    }
    *this = result;
    return *this;
   
}

/*
//  Modulus Exponentiation assignment function.
//     - Compute a = (a ^ b) % m.
//     - Do NOT compute a^b directly and then % m. 
//         This is too computationally expensive.
*/
const BigInt & BigInt::modulusExp(const BigInt &b, const BigInt &m){
    if(base != b.base || base != m.base){
        throw DiffBaseException();
    }
    if(!b.isPositive){
        throw ExpByNegativeException();
    }
    /************* You complete *************/
    
    BigInt result(1, base), exp = b, zero(0, base), two(2, base);
    BigInt one (1, base);
    
    while (exp > zero) {
        if ((exp % two) == zero) {
            *this *= *this;
            *this%=m;
            exp/=two;
        }
        else {
            exp-=one;
            result *= *this;
            result%=m;
        }
    }
    *this = result;
    return *this;
    
}


//----------------------------------------------------
//* Operator overloading, non-member functions
//----------------------------------------------------


//Call the += function above to compute a BigInt whose value is a + b
BigInt operator + (const  BigInt &a, const BigInt & b){

    BigInt c = a;
    c += b;
    return c;                                                                          
                                                                     
}
//Call the -= function above to compute a BigInt whose value is a - b
BigInt operator - (const  BigInt &a, const BigInt & b){

    /************* You complete *************/
    BigInt c = a;
    c-=b;
    return c;
  
}
//Call the *= function above to compute a BigInt whose value is a * b
BigInt operator * (const  BigInt &a, const BigInt & b){

    /************* You complete *************/
    BigInt temp = a;
    temp *= b;
    return temp;
  
}

//Call the /= function above to compute a BigInt whose value is a / b
BigInt operator / (const  BigInt &a, const BigInt & b){

    /************* You complete *************/
    
    BigInt temp = a;
    temp /= b;
    return temp;

}

//Call the %= function above to compute a BigInt whose value is a % b
BigInt operator % (const  BigInt &a, const BigInt & b){

    /************* You complete *************/
  
    BigInt temp = a;
    temp %= b;
    
    return temp;

}
//Call the exponentiation function above to compute a BigInt whose value is pow(a,b)
BigInt pow(const  BigInt &a, const BigInt & b){

    /************* You complete *************/
    BigInt temp = a;
    temp.exponentiation(b);
    return temp;
  
}

//Call the modulusExp function above to compute a BigInt whose value is (a ^ b) mod c
BigInt modPow(const BigInt &a, const BigInt &b, const BigInt &m){

    /************* You complete *************/
  BigInt temp = a;
    temp.modulusExp(b,m);
    return temp;
 
}

//Call the compare function above to check if a == b
bool operator == (const BigInt &a, const BigInt &b){
  
    int isEqual;
    isEqual = a.compare(b);

    if (isEqual == 0) {
        return true;
    }
    else {
        return false;
    }

}

//Call the compare function above to check if a != b
bool operator != (const BigInt &a, const BigInt &b){
  
    int isEqual;
    isEqual = a.compare(b);

    if (isEqual == 0) {
        return false;
    }
    else {
        return true;
    }

}

//Call the compare function above to check if a <= b
bool operator <= (const BigInt &a, const BigInt &b){
  
    int isEqual;
    isEqual = a.compare(b);

    if (isEqual == 0 || isEqual == -1) {
        return true;
    }
    else {
        return false;
    }

}

//Call the compare function above to check if a >= b
bool operator >= (const BigInt &a, const BigInt &b){
  
    int isEqual;
    isEqual = a.compare(b);

    if (isEqual == 0 || isEqual == 1) {
        return true;
    }
    else {
        return false;
    }
  
}

//Call the compare function above to check if a > b
bool operator > (const BigInt &a, const BigInt &b){
  
    int isEqual;
    isEqual = a.compare(b);

    if (isEqual == 1) {
        return true;
    }
    else {
        return false;
    }

}

//Call the compare function above to check if a < b
bool operator < (const BigInt &a, const BigInt &b){
  
    int isEqual;
    isEqual = a.compare(b);

    if (isEqual == -1) {
        return true;
    }
    else {
        return false;
    }


}