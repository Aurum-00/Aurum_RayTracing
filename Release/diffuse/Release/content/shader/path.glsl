// ·��׷��
vec3 pathTracing(HitResult hit, int maxBounce) {

    vec3 Lo = vec3(0);      // ���յ���ɫ
    vec3 history = vec3(1); // �ݹ���۵���ɫ

    for(int bounce=0; bounce<maxBounce; bounce++) {
        // ������䷽�� wi
        vec3 wi = toNormalHemisphere(SampleHemisphere(), hit.normal);

        // ������: ����������
        Ray randomRay;
        randomRay.startPoint = hit.hitPoint;
        randomRay.direction = wi;
        HitResult newHit = hitBVH(randomRay);

        float pdf = 1.0 / (2.0 * PI);                                   // ������Ȳ��������ܶ�
        float cosine_o = max(0, dot(-hit.viewDir, hit.normal));         // �����ͷ��߼н�����
        float cosine_i = max(0, dot(randomRay.direction, hit.normal));  // �����ͷ��߼н�����

        vec3 f_r = hit.material.baseColor / PI;                         // ������ BRDF

        // δ����
        if(!newHit.isHit) {
            vec3 skyColor = sampleHdr(randomRay.direction);
            Lo += history * skyColor * f_r * cosine_i / pdf;
            break;
        }
        
        // ���й�Դ������ɫ
        vec3 Le = newHit.material.emissive;
        Lo += history * Le * f_r * cosine_i / pdf;
        
        // �ݹ�(����)
        hit = newHit;
        history *= f_r * cosine_i / pdf;  // �ۻ���ɫ
    }
    
    return Lo;
}

// ·��׷��
vec3 pathTracing(HitResult hit, int maxBounce) 
{

    vec3 Lo = vec3(0);      // ���յ���ɫ
    vec3 history = vec3(1); // �ݹ���۵���ɫ

    for(int bounce=0; bounce<maxBounce; bounce++) {
        // ������䷽�� wi
        vec3 wi = toNormalHemisphere(SampleHemisphere(), hit.normal);


        // ������: ����������
        Ray randomRay;
        randomRay.startPoint = hit.hitPoint;
        randomRay.direction = wi;
        HitResult newHit = hitBVH(randomRay);

        //V �����
        vec3 V = -hit.viewDir;
        //N ����
        vec3 N = hit.normal;
        //H �������
        vec3 H = normalize(V + L);
        //L �����
        vec3 L = wi;

        //���㷨�߷ֲ�����NDF
        float a = hit.material.roughness * hit.material.roughness;
        float a2 = a * a;
        float NdotH = max(dot(N, H), 0.0);
        float NdotH2 = NdotH * NdotH;
        float nom   = a2;
        float denom = (NdotH2 * (a2 - 1.0) + 1.0);
        denom = PI * denom * denom;
        float NDF = nom / denom;

        //���㼸����G
        float NdotV = max(dot(N, V), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float ggx2 = GeometrySchlickGGX(NdotV, roughness);
        float ggx1 = GeometrySchlickGGX(NdotL, roughness);
        float G = ggx1 * ggx2;

        //�����������
        vec3 albedo = vec3(0.5f, 0.0f, 0.0f);
        vec3 F0 = vec3(0.04); 
        F0 = mix(F0, albedo, material.metallic);
        float cosTheta = clamp(dot(H, V)
        vec3 F = F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);

        vec3 numerator = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - material.metallic;


        float pdf = 1.0 / (2.0 * PI);                                   // ������Ȳ��������ܶ�
        float cosine_o = max(0, dot(-hit.viewDir, hit.normal));         // �����ͷ��߼н�����
        float cosine_i = max(0, dot(randomRay.direction, hit.normal));  // �����ͷ��߼н�����

        vec3 f_r = kD * hit.material.baseColor / PI + specular;                         // ������ BRDF

        // δ����
        if(!newHit.isHit) {
            vec3 skyColor = sampleHdr(randomRay.direction);
            Lo += history * skyColor * f_r * cosine_i / pdf;
            break;
        }
        
        // ���й�Դ������ɫ
        vec3 Le = newHit.material.emissive;
        Lo += history * Le * f_r * cosine_i / pdf;
        
        // �ݹ�(����)
        hit = newHit;
        history *= f_r * cosine_i / pdf;  // �ۻ���ɫ
    }
    
    return Lo;
}

// ·��׷��-�޸ĺ�
vec3 pathTracing(HitResult hit, int maxBounce) 
{

    vec3 Lo = vec3(0);      // ���յ���ɫ
    vec3 history = vec3(1); // �ݹ���۵���ɫ

    for(int bounce=0; bounce<maxBounce; bounce++) {
        // ������䷽�� wi
        vec3 wi = toNormalHemisphere(SampleHemisphere(), hit.normal);


        // ������: ����������
        Ray randomRay;
        randomRay.startPoint = hit.hitPoint;
        randomRay.direction = wi;
        HitResult newHit = hitBVH(randomRay);

        //V �����
        vec3 V = -hit.viewDir;
        //N ����
        vec3 N = hit.normal;
        //L �����
        vec3 L = wi;
        //H �������
        vec3 H = normalize(V + L);


        //���㷨�߷ֲ�����NDF
        float a = hit.material.roughness * hit.material.roughness;
        float a2 = a * a;
        float NdotH = max(dot(N, H), 0.0);
        float NdotH2 = NdotH * NdotH;
        float nom   = a2;
        float denom = (NdotH2 * (a2 - 1.0) + 1.0);
        denom = PI * denom * denom;
        float NDF = nom / denom;

        //���㼸����G
        float NdotV = max(dot(N, V), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float ggx2 = GeometrySchlickGGX(NdotV, hit.material.roughness);
        float ggx1 = GeometrySchlickGGX(NdotL, hit.material.roughness);
        float G = ggx1 * ggx2;

        //�����������
        vec3 albedo = vec3(0.5f, 0.0f, 0.0f);
        vec3 F0 = vec3(0.04); 
        F0 = mix(F0, albedo, hit.material.metallic);
        float cosTheta = clamp(dot(H, V), 0.0, 1.0);
        vec3 F = F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);

        vec3 numerator = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - hit.material.metallic;


        float pdf = 1.0 / (2.0 * PI);                                   // ������Ȳ��������ܶ�
        float cosine_o = max(0, dot(-hit.viewDir, hit.normal));         // �����ͷ��߼н�����
        float cosine_i = max(0, dot(randomRay.direction, hit.normal));  // �����ͷ��߼н�����

        vec3 f_r = kD * hit.material.baseColor / PI + specular;                         // ������ BRDF

        // δ����
        if(!newHit.isHit) {
            vec3 skyColor = sampleHdr(randomRay.direction);
            Lo += history * skyColor * f_r * cosine_i / pdf;
            break;
        }
        
        // ���й�Դ������ɫ
        vec3 Le = newHit.material.emissive;
        Lo += history * Le * f_r * cosine_i / pdf;
        
        // �ݹ�(����)
        hit = newHit;
        history *= f_r * cosine_i / pdf;  // �ۻ���ɫ
    }
    
    return Lo;
}