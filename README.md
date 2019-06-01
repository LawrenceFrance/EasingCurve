# EasingCurve
Curve interpolator

Easing Curves Calculator
Lawrence France
Started 30/03/19

Description

	Output easing curve data given curve parameters and input data from user

	Input: 	Desired easing curve and associated parameters
				These may be:
					Linear
					InQuad
					OutQuad
					InOutQuad
					(information at http://doc.qt.io/qt-5/qeasingcurve.html#Type-enum)
			Time value at which easing curve should be evaluated

			Program deals with malformed inputs, e.g. incomplete variables
			Also performs validation checks, e.g. non-number inputs
			These are discarded until user enters valid input
			

	Output:		Output easing curve details back to user
				Value of easing curve at user given time value 

	Calculation details:

		Using notation:
			diff 	= x(duration) - x(0)

		Example easing curve parameters:
			x(0)		= 100
			x(duration)	= 200
			d 			= 1.0
			diff 		= 100

	
		In general, Output = x(0) + diff * (t/d)^n
												^ "easing factor"


		For linear, n=1:
			f(t) = x(0) + diff * t/d
		Example inputs and their outputs:
				t = 0.2 --> f(t) = 120
				t = 0.5 --> f(t) = 150
				t = 1.0 --> f(t) = 200

		For InQuad, n=2:
			f(t) = x(0) + diff * (t/d)^2
		Example inputs and their outputs:
				t = 0.2 --> f(t) = 104
				t = 0.5 --> f(t) = 125
				t = 1.0 --> f(t) = 200

		For OutQuad, n=2 with an inversion:
			f(t) = x(duration) - diff * (1 - (1 - t/d)^2)
		Example inputs and their outputs:
				t = 0.2 --> f(t) = 136
				t = 0.5 --> f(t) = 175
				t = 1.0 --> f(t) = 200

		For InOutQuad:

			if t between 0<->d/2, use InQuad, e.g.  x(0) = 100, x(duration) = 150 :
				t = 0.2 --> f(t) = 108
				t = 0.4 --> f(t) = 132

			if t between d/2<->d, use OutQuad, e.g. x(0) = 150, x(duration) = 200 :
				t = 0.7 --> f(t) = 168
				t = 1.0 --> f(t) = 200

			if t == d/2, we know f(0.5) = x(0) + diff/2 = 150 :
				f(t) = x(0) + diff / 2
