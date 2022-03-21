//Rachneet Kaur
// IE 531 End Term Programming Assignment
// Edo Liberty Matrix Sketching Algorithm

// If A is m*n matrix, where n>>m then Matrix sketch is m*l, l<n
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include "include.h"
#include "newmat.h"
#include "newmatio.h"
#include "newmatap.h"
using namespace std;

double Frobenious_Norm(Matrix Data)  // To compute Frobenius norm of A*A.(t)
{
	Data = Data*Data.t();
	double norm = Data.NormFrobenius();
	return norm;	
}

Matrix Matrix_Sketch(Matrix Data, double epsilon)
{
	int cols_of_sketch = ceil(2.0 / epsilon);  // l = 2/Epsilon
	cout << "l= " << cols_of_sketch << endl;
	IdentityMatrix I(cols_of_sketch);
	Matrix U, V;
	DiagonalMatrix D;   // For SVD
	double del;
	// If case: If no. of columns in matrix sketch (l) < No. of rows (m) then Matrix Sketch is a m*l matrix
	if (cols_of_sketch < Data.Nrows())  // No. of columns in Matrix Sketch (B) < No. of rows (l<m)
	{
		Matrix Result(Data.Nrows(), cols_of_sketch), Sigma(cols_of_sketch, cols_of_sketch);  // New Matrix Result (B) is m*l matrix
		Result = 0.0;
		Sigma = 0.0;
		for (int s = 1; s <= Data.Ncols(); s++) 
		{
		// Copying column of Data matrix (A) to last column of Result matrix (B)
		Result.Column(cols_of_sketch) = Data.Column(s);
		SVD(Result, D, U, V);  // Computing SVD
		del = D(cols_of_sketch)*D(cols_of_sketch);  // Square of least singular value
		// Reducing each singular value by least singular value
		for (int k = 1; k <= cols_of_sketch; k++)
			Sigma(k, k) = pow(D(k)*D(k) - del, 0.5);  // Here last element of sigma is 0
		Result = U*Sigma;  // Constructing Result by U*Sigma
		}
		return Result;
	}

	// Else case: If no. of columns in matrix sketch (l) > No. of rows (m) then Matrix Sketch is a m*m matrix
	else     // No. of columns in Matrix Sketch (B) > No. of rows (l>m)
	{
		Matrix Result(Data.Nrows(), Data.Nrows()), Sigma(Data.Nrows(), Data.Nrows()); // New Matrix Result is m*m matrix
		Result = 0.0;
		Sigma = 0.0;
		SVD(Data*Data.t(), D, U, V);   // SVD of A*A.t()
		for (int i = 1; i <= Data.Nrows(); i++)
			Sigma(i, i) = pow(D(i), 0.5);  // Computing square root of each entry in Diagonal Matrix D
		Result = U*Sigma*V.t();  // Constructing Result matrix by U*sigma*V.t()
		return Result;
	}
}

int main(int argc, char* argv[])
{
	int dimension, no_of_data_points;
	double epsilon;

	sscanf(argv[1], "%d", &dimension);
	sscanf(argv[2], "%d", &no_of_data_points);
	sscanf(argv[3], "%lf", &epsilon);
	ifstream input_file(argv[4]);
	ofstream output_file(argv[5]);

	Matrix Data(dimension, no_of_data_points);

	cout << "Edo Liberty's Matrix Sketching Algorithm" << endl;
	cout << "----------------------------------------" << endl;
	cout << "Original Data-Matrix has " << dimension << "-rows & " << no_of_data_points << "-cols" << endl;
	cout << "Epsilon = " << epsilon << " (i.e. max. of " << 100 * epsilon << "% reduction of  Frobenius-Norm of the Sketch Matrix)" << endl;
	cout << "Input File = " << argv[4] << endl;

	// Read the Data
	for (int i = 1; i <= dimension; i++)
		for (int j = 1; j <= no_of_data_points; j++)
		{
			double x;
			input_file >> x;
			Data(i, j) = x;
		}

	// Compute the Frobenius-Norm of the original Data-Matrix
	double Data_Forbenius_Norm = Frobenious_Norm(Data);
	cout << "Frobenius Norm of the (" << Data.Nrows() << " x " << Data.Ncols() << ") Data Matrix = ";
	cout << Data_Forbenius_Norm << endl;

	Matrix Sketch(dimension, min(dimension, (int)ceil(2 / epsilon)));
	Sketch = Matrix_Sketch(Data, epsilon);
	double Sketch_Forbenius_Norm = Frobenious_Norm(Sketch);
	cout << "Frobenius Norm of the (" << Sketch.Nrows() << " x " << Sketch.Ncols() << ") Sketch Matrix = ";
	cout << Sketch_Forbenius_Norm << endl;
	cout << "Change in Frobenius-Norm between Sketch & Original  = ";
	cout << setprecision(3) << 100 * (Sketch_Forbenius_Norm - Data_Forbenius_Norm) / Data_Forbenius_Norm << "%" << endl;

	output_file << Sketch;
	cout << "File `" << argv[5] << "' contains a (" << Sketch.Nrows() << " x " << Sketch.Ncols();
	cout << ") Matrix-Sketch" << endl;


}
