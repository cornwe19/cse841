/**
 * @file    vector.cc
 * @author  Austin Gregory
 * @created 2013-11-04
 */

#include "vector.hpp"

Vector::Vector(unsigned size)
{
	initialize(size, "");
}

Vector::Vector(unsigned size, string label)
{
	initialize(size, label);
}

Vector::Vector(const Vector& vector)
{
	initialize(vector._size, vector._label);
	
	for (unsigned i = 0; i < vector._size; ++i)
	{
		_data[i] = vector._data[i];
	}
}

Vector::~Vector()
{
	dispose();
}





Vector& Vector::operator=(const Vector& vector)
{
	if (this != &vector)
	{
		dispose();
		initialize(vector._size, _label);
		
		for (unsigned i = 0; i < _size; ++i)
		{
			_data[i] = vector._data[i];
		}
	}

	return *this;
}

Vector& Vector::operator+=(const Vector& vector)
{
	if (_size != vector._size)
	{
		cerr << "\n\nException: Attempted the += operator with two vectors of unequal size.\n";
		cerr << "Left Operand Label: " << _label << endl;
		cerr << "Right Operand Label: " << vector._label << endl << endl;
	}
	
	for (unsigned i = 0; i < _size; ++i)
	{
		_data[i] += vector._data[i];
	}

	return *this;
}

Vector& Vector::operator-=(const Vector& vector)
{
	if (_size != vector._size)
	{
		cerr << "\n\nException: Attempted the -= operator with two vectors of unequal size.\n";
		cerr << "Left Operand Label: " << _label << endl;
		cerr << "Right Operand Label: " << vector._label << endl << endl;
	}
	
	for (unsigned i = 0; i < _size; ++i)
	{
		_data[i] -= vector._data[i];
	}

	return *this;
}

Vector& Vector::operator*=(double multiplier)
{
	for (unsigned i = 0; i < _size; ++i)
	{
		_data[i] *= multiplier;
	}

	return *this;
}

Vector& Vector::operator/=(double denominator)
{
	for (unsigned i = 0; i < _size; ++i)
	{
		_data[i] /= denominator;
	}

	return *this;
}





Vector operator+(const Vector& left, const Vector& right)
{
	Vector returnVector(left);
	returnVector._label = "Temporary";

	returnVector += right;

	return returnVector;
}

Vector operator-(const Vector& left, const Vector& right)
{
	Vector returnVector(left);
	returnVector._label = "Temporary";

	returnVector -= right;

	return returnVector;
}

Vector operator*(const Vector& vector, double multiplier)
{
	Vector returnVector(vector);
	returnVector._label = "Temporary";

	returnVector *= multiplier;

	return returnVector;
}

Vector operator*(double multiplier, const Vector& vector)
{
	return vector * multiplier;
}

Vector operator/(const Vector& vector, double denominator)
{
	Vector returnVector(vector);
	returnVector._label = "Temporary";

	returnVector /= denominator;

	return returnVector;
}





ostream& operator<<(ostream& outStream, const Vector& vector)
{
	if (vector._size > 0)
	{
		outStream << vector._data[0];
		
		for (unsigned i = 1; i < vector._size; ++i)
		{
			outStream << ", " << vector._data[i];
		}
	}
	
	return outStream;
}





void Vector::print(ostream& outStream)
{
	outStream << *this;
}





double& Vector::at(unsigned index)
{
	return _data[index];
}

void Vector::setLabel(string label)
{
	_label = label;
}





double Vector::at(unsigned index) const
{
	return _data[index];
}

unsigned Vector::getSize() const
{
	return _size;
}

string Vector::getLabel() const
{
	return _label;
}





void Vector::randomize(double minimum, double maximum)
{
	if (minimum > maximum)
	{
		cerr << "\n\nException: Minimum is greater than maximum in randomize method.\n";
		cerr << "Minimum: " << minimum << endl;
		cerr << "Maximum: " << maximum << endl << endl;
	}
	
	if (minimum == maximum)
	{
		assign(minimum);
	}
	else
	{	
		srand(time(NULL));

		for (unsigned i = 0; i < _size; ++i)
		{
			_data[i] = fmod(rand(), (maximum - minimum + 1)) + minimum;
		}
	}
}

void Vector::assign(double value)
{
	for (unsigned i = 0; i < _size; ++i)
	{
		_data[i] = value;
	}
}





double Vector::dot(const Vector& vector) const
{
	if (_size != vector._size)
	{
		cerr << "\n\nException: Attempted method dot with a vector of unequal size.\n";
		cerr << "Calling Object Label: " << _label << endl;
		cerr << "Parameter Label: " << vector._label << endl << endl;
	}
	
	double dotProduct = 0;

	for (unsigned i = 0; i < _size; ++i)
	{
		dotProduct += _data[i] * vector._data[i];
	}

	return dotProduct;
}

double Vector::calculateNorm() const
{
	double norm = 0;

	for (unsigned i = 0; i < _size; ++i)
	{
		norm += _data[i] * _data[i];
	}

	norm = sqrt(norm);

	return norm;
}

double Vector::findMinimumValue() const
{
	double minimumValue = _data[0];
	
	for (unsigned i = 1; i < _size; ++i)
	{
		if (_data[i] < minimumValue)
		{
			minimumValue = _data[i];
		}
	}
	
	return minimumValue;
}

double Vector::findMaximumValue() const
{
	double maximumValue = _data[0];
	
	for (unsigned i = 1; i < _size; ++i)
	{
		if (_data[i] > maximumValue)
		{
			maximumValue = _data[i];
		}
	}
	
	return maximumValue;
}

double Vector::calculateDistanceTo(const Vector& vector) const
{
	if (_size != vector._size)
	{
		cerr << "\n\nException: Attempted to find distance to a vector of different size.\n";
		cerr << "Calling Object Label: " << _label << endl;
		cerr << "Parameter Label: " << vector._label << endl << endl;
	}
	
	double distance = 0;
	
	for (unsigned i = 0; i < _size; ++i)
	{
		distance += (vector._data[i] - _data[i]) * (vector._data[i] - _data[i]);
	}
	
	distance = sqrt(distance);
	
	return distance;
}





Vector::Vector()
{
}





void Vector::initialize(unsigned size, string label)
{
	_data = new double[size];
	_size = size;
	_label = label;

	for (unsigned i = 0; i < _size; ++i)
	{
		_data[i] = 0;
	}
}

void Vector::dispose()
{
	delete _data;
	
	_size = 0;
	_label = "";
}
