I developed a BigInt library that supports adding positive numbers that can be arbitrarily large. To that end, that BigInt library is still too simple to do anything useful with it.
The goal is to create a BigInt library that can
1). Support arbitrary precision signed arithmetic.
We want to be able to perform basic arithmetic operations on positive and negative BigInts such as addition, multiplication, subtraction, division, modulus, exponentiation, etc. as well as checking for equality.
2). Support operator overloads for C++ math and comparison.
We want to use BigInts just like regular integers. We can do so by overloading C++ built-in operators (+=, /=, ==, >=, <=, !=, +, -, %, etc).
3). Support numbers of any base between 2 and 36.
We want the library to handle many different number systems, not just the base-10 number system. After all, there is nothing too exciting about just numbers in base 10, except that the median human has that many fingers.
Additionally, we want the BigInt library to perform the arithmetic operations efficiently. After all, no one wants to have to wait for a long time to exponentiate two numbers, for example. 
