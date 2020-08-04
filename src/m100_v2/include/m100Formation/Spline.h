//
// Created by stone on 18-7-3.
//

#ifndef CARMOVE_SPLINE_H
#define CARMOVE_SPLINE_H
#pragma once

#include <vector>
#include <cmath>
#include <m100Formation/inf_typedef.h>


//�Զ���ĵ�����
//struct MYPOINT
//{
//	float x, y, z;
//
//};

//�������ɲ��������β�ֵ��������
class Spline {

public:

    //���ɵ����ߵ�
    std::vector<float> spline_u;

    Point3D begin_k1;//��㵼ʸ
    Point3D end_k1;//�յ㵼ʸ
    int interval;//�������ߵ������
public:
    //�յĹ��캯����������Ժ����setVertex�����ò�ֵ��
    Spline() {}
    //���ò�ֵ��
    void setVertex(std::vector<Point3D> point, int interval_);

    //���ߵ㵼ʸ
    std::vector<Point3D> daoshi;
    //��õ�
    std::vector<Point3D> get_point();

    std::vector<Point3D> spline_create(int i = 2);

private:
    std::vector<Point3D> m_aVertex;//��ֵ��,��ĿΪnum
    std::vector<float> distance;//�ҳ�(num-1)
    std::vector<float> distance_sum_n;
    std::vector<float> canshu_u;//��ֵ��Ĳ���ֵ(num)
    std::vector<float> delta;//delta(num-1)
    std::vector<Point3D> all_point;//���ߵ�

    std::vector<std::vector<float>>  matrix_m;
    std::vector<std::vector<float>>  matrix_right;

    //��ֵ�㵼ʸ
    std::vector<std::vector<float>>  daoshi_vec;
    std::vector<Point3D> _p;

    std::vector<float> m_Cx;//ÿ����ֵ��ĵ�ʸ��x
    std::vector<float> m_Cy;//ÿ����ֵ��ĵ�ʸ��y
    std::vector<float> m_Cz;//ÿ����ֵ��ĵ�ʸ��z

private:
    //�淶�����ҳ�������
    void parameterization();

    void setMatrix(int condition);

    void getDerivative();

    void tridia(int n, int k, std::vector<std::vector<float>>&dia, std::vector<std::vector<float>>&t,
                int iflag);

    void getSplineDerivat(int i, float u, int j, Point3D &p);

    void getCubicCuvValue(int i, std::vector<float> &tx, std::vector<float> &ty, std::vector<float> &tz, float u,
                          int j, Point3D &p);

};
#endif //CARMOVE_SPLINE_H
