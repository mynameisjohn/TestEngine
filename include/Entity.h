#ifndef ENTITY_H
#define ENTITY_H

#include "Collider.h"
#include "Skeleton.h"

class Entity{
	public:
		Entity();
		Entity(Entity *);
		Entity(vec3 translate, vec3 scale);
		Entity(Collider c, Skeleton s);
		Entity(Collider c);
		void setCol(Collider c);
		void setSkeleton(Skeleton s);
		void setBB(BoundBox bb);
		void translate(vec3);
		void addDrawable(unique_ptr<Drawable> dr);
		void draw();//int MVHandle, int ColorHandle);
		char collidesWith(Entity * e);
		bool overlapsWith(Entity * e);
		float toLeft(Entity *);	
		float toRight(Entity *);
		float toBottom(Entity *);
		float toTop(Entity *);
		float toNear(Entity *);
		float toFar(Entity *);
		vec3 getPos();
		vec3 center();
		virtual vec3 getPotential(vec3 pos);
		virtual void update(){}
		Collider * getCollider(){return &mCollider;}
		Skeleton * getSkeleton(){return &mSkeleton;}
	protected:
		Collider mCollider;
		vector<unique_ptr<Drawable> > mDrawables;
		Skeleton mSkeleton;
//		SceneGraph mSkeleton;
		vec3 mTrans;
		float sigmaSq, A;
	/* These will be inlined when the time comes
	public:
		inline void translate(vec3 trans){
			mCollider.translate(trans);
		}
		inline vec3 getPos(){
			return mCollider.getPos();
		}
		inline int toLeft(Entity * e){
			return mCollider.toLeft(e.mCollider);
		}
		inline int toRight(Entity * e){
         return mCollider.toRight(e.mCollider);
      }
		inline int toBottom(Entity * e){
         return mCollider.toBottom(e.mCollider);
      }
		inline int toTop(Entity * e){
         return mCollider.toTop(e.mCollider);
      }
		inline int toNear(Entity * e){
         return mCollider.toNear(e.mCollider);
      }
		inline int toFar(Entity * e){
         return mCollider.toFar(e.mCollider);
      }
		inline vec3 center(){
			return mCollider.center();
		}
*/
};


#endif
