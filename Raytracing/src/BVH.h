#pragma once

#include "Model.h"

// BVH ���ڵ�
struct BVHNode {
    int left, right;    // ������������
    //���ﱣ��һ�� n �� һ�� index������ n ��Ϊ 0 ʱ��ʾ��Ҷ�ӽڵ㣬n �Ǵ洢�������εĸ�������triangles ������[index, index + n - 1] ��Χ�������ζ����ڸýڵ�
    int n, index;       // Ҷ�ӽڵ���Ϣ               
    vec3 AA, BB;        // ��ײ��
};

//BVH���ر���
struct BVHNode_encoded 
{
    vec3 childs;        // (left, right, ����)
    vec3 leafInfo;      // (n, index, ����)
    vec3 AA, BB;
};


// ������������������ -- �ȽϺ���
bool cmpx(const Triangle& t1, const Triangle& t2) 
{
    vec3 center1 = (t1.p1 + t1.p2 + t1.p3) / vec3(3, 3, 3);
    vec3 center2 = (t2.p1 + t2.p2 + t2.p3) / vec3(3, 3, 3);
    return center1.x < center2.x;
}
bool cmpy(const Triangle& t1, const Triangle& t2) 
{
    vec3 center1 = (t1.p1 + t1.p2 + t1.p3) / vec3(3, 3, 3);
    vec3 center2 = (t2.p1 + t2.p2 + t2.p3) / vec3(3, 3, 3);
    return center1.y < center2.y;
}
bool cmpz(const Triangle& t1, const Triangle& t2) 
{
    vec3 center1 = (t1.p1 + t1.p2 + t1.p3) / vec3(3, 3, 3);
    vec3 center2 = (t2.p1 + t2.p2 + t2.p3) / vec3(3, 3, 3);
    return center1.z < center2.z;
}

//�Զ���min����
float min(float a, float b) {
    return a < b ? a : b;
}

// �Զ���max����
float max(float a, float b) {
    return a > b ? a : b;
}

// ���� BVH
int buildBVH(std::vector<Triangle>& triangles, std::vector<BVHNode>& nodes, int l, int r, int n) {
    if (l > r) return 0;

    // ע��
    // �˴�����ͨ��ָ�룬���õȷ�ʽ������������ nodes[id] ������
    // ��Ϊ std::vector<> ����ʱ�´����������ڴ棬��ô��ַ�͸ı���
    // ��ָ�룬���þ�ָ��ԭ�����ڴ棬���Իᷢ������
    nodes.push_back(BVHNode());
    int id = nodes.size() - 1;   // ע�⣺ �ȱ�������
    nodes[id].left = nodes[id].right = nodes[id].n = nodes[id].index = 0;
    nodes[id].AA = vec3(1145141919, 1145141919, 1145141919);
    nodes[id].BB = vec3(-1145141919, -1145141919, -1145141919);

    // ���� AABB
    for (int i = l; i <= r; i++) 
    {
        // ��С�� AA
        float minx = min(triangles[i].p1.x, min(triangles[i].p2.x, triangles[i].p3.x));
        float miny = min(triangles[i].p1.y, min(triangles[i].p2.y, triangles[i].p3.y));
        float minz = min(triangles[i].p1.z, min(triangles[i].p2.z, triangles[i].p3.z));
        nodes[id].AA.x = min(nodes[id].AA.x, minx);
        nodes[id].AA.y = min(nodes[id].AA.y, miny);
        nodes[id].AA.z = min(nodes[id].AA.z, minz);
        // ���� BB
        float maxx = max(triangles[i].p1.x, max(triangles[i].p2.x, triangles[i].p3.x));
        float maxy = max(triangles[i].p1.y, max(triangles[i].p2.y, triangles[i].p3.y));
        float maxz = max(triangles[i].p1.z, max(triangles[i].p2.z, triangles[i].p3.z));
        nodes[id].BB.x = max(nodes[id].BB.x, maxx);
        nodes[id].BB.y = max(nodes[id].BB.y, maxy);
        nodes[id].BB.z = max(nodes[id].BB.z, maxz);
    }

    // ������ n �������� ����Ҷ�ӽڵ�
    if ((r - l + 1) <= n) 
    {
        nodes[id].n = r - l + 1;
        nodes[id].index = l;
        return id;
    }

    // ����ݹ齨��
    float lenx = nodes[id].BB.x - nodes[id].AA.x;
    float leny = nodes[id].BB.y - nodes[id].AA.y;
    float lenz = nodes[id].BB.z - nodes[id].AA.z;
    // �� x ����
    if (lenx >= leny && lenx >= lenz)
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpx);
    // �� y ����
    if (leny >= lenx && leny >= lenz)
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpy);
    // �� z ����
    if (lenz >= lenx && lenz >= leny)
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpz);
    // ����
    int mid = (l + r) / 2;
    int left = buildBVH(triangles, nodes, l, mid, n);
    int right = buildBVH(triangles, nodes, mid + 1, r, n);

    nodes[id].left = left;
    nodes[id].right = right;

    return id;
}
