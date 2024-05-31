#include "BSplineRandomizer.h"

#include <glm/common.hpp>

BSplineRandomizer::BSplineRandomizer()
    : m_gen(m_rd()), m_uniformZeroToOne(0.0f, 1.0f)
{}

BSplineRandomizer::~BSplineRandomizer()
{}

BSplineRandomizer::BSplineRandomizer(
    const glm::vec3 p0, 
    const glm::vec3 p1, 
    const glm::vec3 p2, 
    const glm::vec3 p3,
    const glm::vec3 pMin,
    const glm::vec3 pMax
    )
    : 
    m_p0{p0}, m_p1{p1}, m_p2{p2}, m_p3{p3}, m_pMin{pMin}, m_pMax{pMax},
    m_gen(m_rd()), m_uniformZeroToOne(0.0f, 1.0f)
{}

glm::vec3 BSplineRandomizer::GetCurvePosition(float t)
{      
    float one_t = 1.0f - t;

    m_deCasteljau[0] = m_p0;
    m_deCasteljau[1] = m_p1;
    m_deCasteljau[2] = m_p2;
    m_deCasteljau[3] = m_p3;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3 - i; j++) {
            m_deCasteljau[j] = one_t * m_deCasteljau[j] + t * m_deCasteljau[j + 1];
        }
    }

    return m_deCasteljau[0];
}

glm::vec3 BSplineRandomizer::GetCurveTangent(float t)
{
    float one_t = 1.0f - t;

    m_deCasteljau[0] = m_p0;
    m_deCasteljau[1] = m_p1;
    m_deCasteljau[2] = m_p2;
    m_deCasteljau[3] = m_p3;

    // derivative 
    for (int i = 0; i < 3; i++) {
        m_deCasteljau[i] = 3.0f * (m_deCasteljau[i + 1] - m_deCasteljau[i]);
    }

    // value of the derivative 
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2 - i; j++) {
            m_deCasteljau[j] = one_t * m_deCasteljau[j] + t * m_deCasteljau[j + 1];
        }
    }
    
    return m_deCasteljau[0];
}

void BSplineRandomizer::GenerateSubsequentCurve()
{
    glm::vec3 newP1, newP2, newP3, newP4,
        a_int, a, b_int, b, c_int, c,      
        d;
    float t;

    d = m_p2 - m_p1; 
    a_int = RayBoxIntersection(m_p2, d);
    t = m_uniformZeroToOne(m_gen);
    a = t * m_p2 + (1 - t) * a_int;

    d = m_p3 - m_p2;
    b_int = RayBoxIntersection(m_p3, d);
    t = m_uniformZeroToOne(m_gen);
    b = t * m_p3 + (1 - t) * b_int;

    d = b - a;
    c_int = RayBoxIntersection(b, d);
    t = m_uniformZeroToOne(m_gen);
    c = t * b + (1 - t) * c_int;

    newP1 = m_p3;
    newP2 = b;
    newP3 = c;
    newP4 = m_pMin + (m_pMax - m_pMin) * 
        glm::vec3(m_uniformZeroToOne(m_gen), 
                  m_uniformZeroToOne(m_gen), 
                  m_uniformZeroToOne(m_gen));

    m_p0 = newP1;
    m_p1 = newP2;
    m_p2 = newP3;
    m_p3 = newP4;
}

glm::vec3 BSplineRandomizer::RayBoxIntersection(glm::vec3 p, glm::vec3 d)
{
    glm::vec3 t1 = (m_pMax - p) / d;
    glm::vec3 t2 = (m_pMin - p) / d;

    glm::vec3 tm = glm::max(t1, t2);
    float t = glm::min(glm::min(tm.x, tm.y), tm.z);

    return p + t * d;
}