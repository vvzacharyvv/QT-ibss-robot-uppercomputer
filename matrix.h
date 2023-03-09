
#ifndef __LIGHT_MATRIX__
#define __LIGHT_MATRIX__

typedef struct  {
	int row, col;
	float **element;
}Mat;

Mat* MatCreate(Mat* mat, int row, int col);
void MatDelete(Mat* mat);
Mat* MatSetVal(Mat* mat, float* val);
void MatDump(const Mat* mat);

Mat* MatZeros(Mat* mat);
Mat* MatEye(Mat* mat);

Mat* MatAdd(Mat* src1, Mat* src2, Mat* dst);
Mat* MatSub(Mat* src1, Mat* src2, Mat* dst);
Mat* MatMul(Mat* src1, Mat* src2, Mat* dst);
Mat* MatTrans(Mat* src, Mat* dst);
float MatDet(Mat* mat);
Mat* MatAdj(Mat* src, Mat* dst);
Mat* MatInv(Mat* src, Mat* dst);

void MatCopy(Mat* src, Mat* dst);
void MatCopy_row(Mat* src, int src_row, Mat* dst, int dst_row);
Mat* MatMul_k(float k, Mat* src1, Mat* dst);

#endif
