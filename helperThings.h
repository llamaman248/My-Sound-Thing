#pragma once

double linearInterpolator(dPoint p1, dPoint p2, double xPosBetween)
{
	double percentP2 = (xPosBetween - p1.time) / (p2.time - p1.time);
	return (percentP2 * p2.value + (1.0 - percentP2) * p1.value);
}


double weightedInterpolator(weightedPoint p1, weightedPoint p2, double xPosBetween)
{
	double percentP2 = (xPosBetween - p1.time) / (p2.time - p1.time);
	double percentP1 = 1.0 - percentP2;
	percentP1 *= p1.weight;
	percentP2 *= p2.weight;
	percentP1 = percentP1 / (percentP1 + percentP2);
	percentP2 = 1.0 - percentP1; // could also be defined as percentP2 = percentP2 / (percentP1 + percentP2), but only if this line of code is before the previous line because it needs the previous value of percentP1

	return (percentP2 * p2.value + percentP1 * p1.value);
}

template<typename T>
class vectorPiece
{
public:
	T value = {};
	vectorPiece<T>* nextPiece = 0;
	vectorPiece() {}
	vectorPiece(T initValue, vectorPiece<T>* nextPiece) : value(initValue), nextPiece(nextPiece) {}
};

template<typename T>
class loopingVector
{
	// Keeps track of current position, Loops back to start from end
public:
	vectorPiece<T>* currentPiece = 0;
	//vectorPiece<T>* startPiece = 0; Planned to have an inheriting class that implements a start.		May also have one that has a stored size and one that alows access to an Nth term
	
	loopingVector(T firstValue)
	{
		currentPiece = new vectorPiece<T>{firstValue, 0};
		currentPiece->nextPiece = currentPiece;
	}

	loopingVector()
	{
		currentPiece = new vectorPiece<T>{};
		currentPiece->nextPiece = currentPiece;
	}

	void pushFromCurrent(T value)
	{
		vectorPiece<T> *oldNext = currentPiece->nextPiece;
		currentPiece->nextPiece = new vectorPiece<T>{value, oldNext};
		//currentPiece = currentPiece->nextPiece;
	}

	void selectNext()
	{
		currentPiece = currentPiece->nextPiece;
	}

	void pushToNext(T value)
	{
		vectorPiece<T>* oldNext = currentPiece->nextPiece;
		currentPiece->nextPiece = new vectorPiece<T>{ value, oldNext };
		currentPiece = currentPiece->nextPiece; // selectNext()
	}

	T next()
	{
		T Treturn = currentPiece->value;
		currentPiece = currentPiece->nextPiece;
		return Treturn;
	}
};