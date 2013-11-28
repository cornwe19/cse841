/**
 * @file    vector.hpp
 * @author  Austin Gregory
 * @created 2013-11-04
 */

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <string>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

class Vector
{
public:
	Vector(unsigned size);
	Vector(unsigned size, string label);
	Vector(const Vector& vector);
	~Vector();

	Vector& operator=(const Vector& vector);
	Vector& operator+=(const Vector& vector);
	Vector& operator-=(const Vector& vector);
	Vector& operator*=(double multiplier);
	Vector& operator/=(double denominator);

	friend Vector operator+(const Vector& left, const Vector& right);
	friend Vector operator-(const Vector& left, const Vector& right);
	friend Vector operator*(const Vector& vector, double multiplier);
	friend Vector operator*(double multiplier, const Vector& vector);
	friend Vector operator/(const Vector& vector, double denominator);
	
	friend ostream& operator<<(ostream& outStream, const Vector& vector);
	
	void print(ostream& outStream);

	double& at(unsigned index);
	void setLabel(string label);
	
	double at(unsigned index) const;
	unsigned getSize() const;
	string getLabel() const;
	
	void randomize(double minimum, double maximum);
	void assign(double value);
	
	double dot(const Vector& vector) const;
	double calculateNorm() const;
	double findMinimumValue() const;
	double findMaximumValue() const;
	double calculateDistanceTo(const Vector& vector) const;

private:
	Vector();
	
	void initialize(unsigned size, string label);
	void dispose();

	double* _data;
	unsigned _size;
	string _label;
};

#endif
