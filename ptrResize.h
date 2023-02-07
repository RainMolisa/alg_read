#pragma once
#include<iot_simd_vector.h>
#define useSimd
template<typename A,typename B>
void ptrResize(A* InIg,int InRows,int InCols,
	B* OtIg,int OtRows,int OtCols);
#ifndef useSimd
template<typename A,typename B>
void ptrResize160(A* InIg,int InRows,int InCols,B* OtIg);
#endif
class resize100160 {
private:
	const int OtRows = 160;
	const int OtCols = 160;
	const int InCols = 100;
	const int InRows = 100;
	const int OtN = OtRows * OtCols;
	//
	float* cAlpha;
	float* rAlpha;
	float* i11;
	float* i21;
	float* i13;
	float* i23;
	int* i11i;
	int* i21i;
	int* i13i;
	int* i23i;
	float* it1;
	float* it2;
	float* ftOtIg;
public:
	resize100160();
	~resize100160();
public:
	void make(uint8_t* InIg, uint8_t* OtIg);
private:
	void initial();
};
void resize100160::make(uint8_t* InIg, uint8_t* OtIg) {
	for (int i = 0; i < OtN; i++) {
		i11[i] = InIg[i11i[i]];
		i21[i] = InIg[i21i[i]];
		i13[i] = InIg[i13i[i]];
		i23[i] = InIg[i23i[i]];
	}
	iot_simd_interpolation_h(i11, i21, rAlpha, it1, OtN);
	iot_simd_interpolation_h(i13, i23, rAlpha, it2, OtN);
	iot_simd_interpolation_h(it1, it2, cAlpha, ftOtIg, OtN);
	iot_simd_fp_to_uint8(OtIg, ftOtIg, OtN);
}
void resize100160::initial() {
	typedef float thArr[3];
	thArr* rInx = new thArr[OtRows];
	thArr* cInx = new thArr[OtCols];
	for (int i = 0; i < OtCols; i++) {
		float val = float(i * InCols) / float(OtCols);
		float intpt = int(val);
		float decpt = val - intpt;
		if (decpt <= 0.0) {
			cInx[i][0] = (decpt);
			cInx[i][1] = intpt;
			cInx[i][2] = intpt;
		}
		else {
			cInx[i][0] = (decpt);
			cInx[i][1] = intpt;
			cInx[i][2] = intpt + 1;
		}
	}
	for (int i = 0; i < OtRows; i++) {
		float val = float(i * InRows) / float(OtRows);
		float intpt = int(val);
		float decpt = val - intpt;
		if (decpt <= 0.0) {
			rInx[i][0] = (decpt);
			rInx[i][1] = intpt;
			rInx[i][2] = intpt;
		}
		else {
			rInx[i][0] = (decpt);
			rInx[i][1] = intpt;
			rInx[i][2] = intpt + 1;
		}
	}
	int otK = 0;
	float inK1 = 0, inK2 = 0, v = 0;
	//
	cAlpha = new float[OtN];
	rAlpha = new float[OtN];
	for (int oy = 0; oy < OtRows; oy++) {
		otK = oy * OtCols;
		for (int ox = 0; ox < OtCols; ox++) {
			cAlpha[otK + ox] = cInx[ox][0];
			rAlpha[otK + ox] = rInx[oy][0];
		}
	}
	i11 = new float[OtN];
	i21 = new float[OtN];
	i13 = new float[OtN];
	i23 = new float[OtN];
	i11i = new int[OtN];
	i21i = new int[OtN];
	i13i = new int[OtN];
	i23i = new int[OtN];
	for (int oy = 0; oy < OtRows; oy++) {
		otK = oy * OtCols;
		inK1 = rInx[oy][1] * InCols;
		inK2 = rInx[oy][2] * InCols;
		for (int ox = 0; ox < OtCols; ox++) {
			i11i[otK + ox] = inK1 + cInx[ox][1];
			i21i[otK + ox] = inK2 + cInx[ox][1];
			i13i[otK + ox] = inK1 + cInx[ox][2];
			i23i[otK + ox] = inK2 + cInx[ox][2];
		}
	}
	it1 = new float[OtN];
	it2 = new float[OtN];
	ftOtIg = new float[OtN];
	delete[] rInx;
	delete[] cInx;
}

resize100160::resize100160() {
	initial();
}

resize100160::~resize100160() {
	delete[] cAlpha;
	delete[] rAlpha;
	delete[] i11;
	delete[] i21;
	delete[] i13;
	delete[] i23;
	delete[] i11i;
	delete[] i21i;
	delete[] i13i;
	delete[] i23i;
	delete[] it1;
	delete[] it2;
	delete[] ftOtIg;
}

#ifdef useSimd
void ptr100resize160(uint8_t* InIg, uint8_t* OtIg) {
	const int OtRows = 160;
	const int OtCols = 160;
	const int OtN = OtRows * OtCols;
	const int InCols = 100;
	const int InRows = 100;
	typedef float biArr[4];
	biArr* rInx = new biArr[OtRows];
	biArr* cInx = new biArr[OtCols];
	for (int i = 0; i < OtCols; i++) {
		float val = float(i * InCols) / float(OtCols);
		float intpt = int(val);
		float decpt = val - intpt;
		if (decpt <= 0.0) {
			cInx[i][0] = (1.0 - decpt);
			cInx[i][1] = intpt;
			cInx[i][2] = decpt;
			cInx[i][3] = intpt;
		}
		else {
			cInx[i][0] = (1.0 - decpt);
			cInx[i][1] = intpt;
			cInx[i][2] = decpt;
			cInx[i][3] = intpt + 1;
		}
	}
	for (int i = 0; i < OtRows; i++) {
		float val = float(i * InRows) / float(OtRows);
		float intpt = int(val);
		float decpt = val - intpt;
		if (decpt <= 0.0) {
			rInx[i][0] = (1.0 - decpt);
			rInx[i][1] = intpt;
			rInx[i][2] = decpt;
			rInx[i][3] = intpt;
		}
		else {
			rInx[i][0] = (1.0 - decpt);
			rInx[i][1] = intpt;
			rInx[i][2] = decpt;
			rInx[i][3] = intpt + 1;
		}
	}
	int otK = 0;
	float inK1 = 0, inK2 = 0, v = 0;
	//
	float* cAlpha = new float[OtN];
	float* rAlpha = new float[OtN];
	for (int oy = 0; oy < OtRows; oy++) {
		otK = oy * OtCols;
		for (int ox = 0; ox < OtCols; ox++) {
			cAlpha[otK + ox] = cInx[ox][0];
			rAlpha[otK + ox] = rInx[oy][0];
		}
	}
	//
	float* i11 = new float[OtN];
	float* i21 = new float[OtN];
	float* i13 = new float[OtN];
	float* i23 = new float[OtN];
	for (int oy = 0; oy < OtRows; oy++) {
		otK = oy * OtCols;
		inK1 = rInx[oy][1] * InCols;
		inK2 = rInx[oy][3] * InCols;
		for (int ox = 0; ox < OtCols; ox++) {
			i11[otK + ox] = InIg[int(inK1 + cInx[ox][1])];
			i21[otK + ox] = InIg[int(inK2 + cInx[ox][1])];
			i13[otK + ox] = InIg[int(inK1 + cInx[ox][3])];
			i23[otK + ox] = InIg[int(inK2 + cInx[ox][3])];
		}
	}
	float* it1 = new float[OtN];
	float* it2 = new float[OtN];
	float* ftOtIg = new float[OtN];
	iot_simd_interpolation_h(i21, i11, rAlpha, it1, OtN);
	iot_simd_interpolation_h(i23, i13, rAlpha, it2, OtN);
	iot_simd_interpolation_h(it2, it1, cAlpha, ftOtIg, OtN);
	//cInx[ox][0] * (rInx[oy][0] * float(InIg[int(inK1 + cInx[ox][1])]) +rInx[oy][2] * float(InIg[int(inK2 + cInx[ox][1])])) +
	//	cInx[ox][2] *( rInx[oy][0] * float(InIg[int(inK1 + cInx[ox][3])]) +rInx[oy][2] * float(InIg[int(inK2 + cInx[ox][3])]));
	iot_simd_fp_to_uint8(OtIg, ftOtIg, OtN);
	
	delete[] rInx;
	delete[] cInx;
	delete[] cAlpha;
	delete[] rAlpha;
	delete[] i11;
	delete[] i21;
	delete[] i13;
	delete[] i23;
	delete[] it1;
	delete[] it2;
	delete[] ftOtIg;
}
#else
template<typename A, typename B>
void ptrResize160(A* InIg, int InRows, int InCols, B* OtIg) {
	ptrResize<A, B>(InIg, InRows, InCols, OtIg, 160, 160);
}
#endif // useSimd


template<typename A,typename B>
void ptrResize(A* InIg,int InRows,int InCols,
	B* OtIg,int OtRows,int OtCols){
	typedef float biArr[4];
	biArr *rInx=new biArr[OtRows];
	biArr *cInx=new biArr[OtCols];
	for(int i=0;i<OtCols;i++){
		float val=float(i*InCols)/float(OtCols);
		float intpt=int(val);
		float decpt=val-intpt;
		if(decpt<=0.0){
			cInx[i][0]=(1.0-decpt);
			cInx[i][1]=intpt;
			cInx[i][2]=decpt;
			cInx[i][3]=intpt;
		}else{
			cInx[i][0]=(1.0-decpt);
			cInx[i][1]=intpt;
			cInx[i][2]=decpt;
			cInx[i][3]=intpt+1;
		}
	}
	for(int i=0;i<OtRows;i++){
		float val=float(i*InRows)/float(OtRows);
		float intpt=int(val);
		float decpt=val-intpt;
		if(decpt<=0.0){
			rInx[i][0]=(1.0-decpt);
			rInx[i][1]=intpt;
			rInx[i][2]=decpt;
			rInx[i][3]=intpt;
		}else{
			rInx[i][0]=(1.0-decpt);
			rInx[i][1]=intpt;
			rInx[i][2]=decpt;
			rInx[i][3]=intpt+1;
		}
	}
	int otK = 0;
	float inK1 = 0, inK2 = 0, v = 0;
	for(int oy=0;oy<OtRows;oy++){
		otK = oy * OtCols;
		inK1 = rInx[oy][1] * InCols;
		inK2 = rInx[oy][3] * InCols;
		for(int ox=0;ox<OtCols;ox++){
			v = cInx[ox][0] * rInx[oy][0] * float(InIg[int(inK1 + cInx[ox][1])]) +
				cInx[ox][0] * rInx[oy][2] * float(InIg[int(inK2 + cInx[ox][1])]) +
				cInx[ox][2] * rInx[oy][0] * float(InIg[int(inK1 + cInx[ox][3])]) +
				cInx[ox][2] * rInx[oy][2] * float(InIg[int(inK2 + cInx[ox][3])]);
			OtIg[otK + ox] = B(v + 0.5);
		}
	}
	delete[] rInx;
	delete[] cInx;
}




/*
template<typename A,typename B>
void ptrResize2(A* InIg,int InRows,int InCols,
	B* OtIg,int OtRows,int OtCols){
	//printf("a\n");
	int *rInx=new int[OtRows];
	int *cInx=new int[OtCols];
	for(int i=0;i<OtCols;i++){
		float val=float(i*InCols)/float(OtCols)+0.5;
		cInx[i]=val;
	}
	for(int i=0;i<OtRows;i++){
		float val=float(i*InRows)/float(OtRows)+0.5;
		rInx[i]=val;
	}
	for(int oy=0;oy<OtRows;oy++){
		int i=oy*OtCols;
		for(int ox=0;ox<OtCols;ox++){
			OtIg[i+ox]=InIg[rInx[oy]*InCols+cInx[ox]];
		}
	}
	delete[] rInx;
	delete[] cInx;
}
*/

/*
template<typename A,typename B>
void ptrResize(A* InIg,int InRows,int InCols,
	B* OtIg,int OtRows,int OtCols){
	for(int oy=0;oy<OtRows;oy++){
		float iy=float(InRows)*(float(oy)/float(OtRows));
		float y0=int(iy);
		float y=iy-y0;
		if(y<=0.0){
			for(int ox=0;ox<OtCols;ox++){
				float ix=float(InCols)*(float(ox)/float(OtCols));
				float x0=int(ix);
				float x=ix-x0;
				//printf("%f x0=%f x=%f\n", ix, x0, x);
				if(x<=0.0){
					OtIg[oy*OtCols+ox]=InIg[int(y0 *InCols+ x0)];
				}else{
					float v = (1.0 - x) * float(InIg[int(y0 * InCols + x0)]) + x * float(InIg[int(y0 * InCols + x0 + 1)]);
					OtIg[oy * OtCols + ox] = B(v+0.5);
				}
			}
		}else{
			for(int ox=0;ox<OtCols;ox++){
				float ix=float(InCols)*(float(ox)/float(OtCols));
				float x0=int(ix);
				float x=ix-x0;
				if(x<=0.0){
					float v= (1.0 - y) * float(InIg[int((y0)*InCols + x0)]) + y * float(InIg[int((y0 + 1) * InCols + x0)]);
					OtIg[oy * OtCols + ox] = B(v + 0.5);
				}else{
					float v=(1.0-x)*(1.0-y)* float(InIg[int((y0 )*InCols+ x0 )])+
						(1.0-x)*y* float(InIg[int((y0+1) *InCols+ x0 )])+
						x*(1.0-y)*float(InIg[int((y0 )*InCols+ x0+1)])+
						x*y*float(InIg[int((y0+1) *InCols+ x0+1)]);
					OtIg[oy*OtCols+ox]= B(v + 0.5);
				}
			}
		}
	}
}
*/


