//
//  PhysicsDebugDraw.cpp
//  DungeonSmith
//
//  Created by The Drudgerist on 7/29/13.
//
//

#include "PhysicsDebugDraw.h"
#include "Locator.h"
#include "Renderer.h"
#include "Primitives3D.h"
#include "Color.h"

PhysicsDebugDraw::~PhysicsDebugDraw() {
}

void PhysicsDebugDraw::drawLine(const btVector3& from,const btVector3& to,
                            const btVector3& fromColor, const btVector3& toColor) {
    Color aColor = RGBAColor(fromColor.getX(), fromColor.getY(), fromColor.getZ(), 1.0f);
    Color bColor = RGBAColor(toColor.getX(), toColor.getY(), toColor.getZ(), 1.0f);
    glm::vec3 a = glm::vec3(from.getX(),from.getY(),from.getZ());
    glm::vec3 b = glm::vec3(to.getX(),to.getY(),to.getZ());
    Locator::getRenderer().DrawPrimitives3D()->Line(a, b, aColor, bColor);
}

void PhysicsDebugDraw::drawLine(const btVector3& from,const btVector3& to,const btVector3& color) {
	drawLine(from,to,color,color);
}

void PhysicsDebugDraw::drawSphere (const btVector3& p, btScalar radius, const btVector3& color)
{
//	glColor4f (color.getX(), color.getY(), color.getZ(), btScalar(1.0f));
//	glPushMatrix ();
//	glTranslatef (p.getX(), p.getY(), p.getZ());
//    
//	int lats = 5;
//	int longs = 5;
//    
//	int i, j;
//	for(i = 0; i <= lats; i++) {
//		btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / lats);
//		btScalar z0  = radius*sin(lat0);
//		btScalar zr0 =  radius*cos(lat0);
//        
//		btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lats);
//		btScalar z1 = radius*sin(lat1);
//		btScalar zr1 = radius*cos(lat1);
//        
//		glBegin(GL_QUAD_STRIP);
//		for(j = 0; j <= longs; j++) {
//			btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / longs;
//			btScalar x = cos(lng);
//			btScalar y = sin(lng);
//            
//			glNormal3f(x * zr0, y * zr0, z0);
//			glVertex3f(x * zr0, y * zr0, z0);
//			glNormal3f(x * zr1, y * zr1, z1);
//			glVertex3f(x * zr1, y * zr1, z1);
//		}
//		glEnd();
//	}
//    
//	glPopMatrix();
}



void PhysicsDebugDraw::drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,
                                const btVector3& color,btScalar alpha) {
    if (m_debugMode > 0) {
        
//		const btVector3	n=btCross(b-a,c-a).normalized();
//		glBegin(GL_TRIANGLES);
//		glColor4f(color.getX(), color.getY(), color.getZ(),alpha);
//		glNormal3d(n.getX(),n.getY(),n.getZ());
//		glVertex3d(a.getX(),a.getY(),a.getZ());
//		glVertex3d(b.getX(),b.getY(),b.getZ());
//		glVertex3d(c.getX(),c.getY(),c.getZ());
//		glEnd();
	}
}

void PhysicsDebugDraw::setDebugMode(int debugMode) {
	m_debugMode = debugMode;
}

void PhysicsDebugDraw::draw3dText(const btVector3& location,const char* textString) {
//	glRasterPos3f(location.x(),  location.y(),  location.z());
	//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
}

void PhysicsDebugDraw::reportErrorWarning(const char* warningString) {
	printf("%s\n",warningString);
}

void PhysicsDebugDraw::drawContactPoint(const btVector3& pointOnB,const btVector3& normalOnB,
                                    btScalar distance,int lifeTime,const btVector3& color) {
	
    btVector3 to=pointOnB+normalOnB*1;//distance;
    const btVector3&from = pointOnB;
    drawLine(from, to, color);
    
    //		glRasterPos3f(from.x(),  from.y(),  from.z());
    //		char buf[12];
    //		sprintf(buf," %d",lifeTime);
    //BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);

}



