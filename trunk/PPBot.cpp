#include "PPBot.h"
#include "PPEnvironment.h"

void PPBot::updateFuturePosition() {                 
	(*_futurePosition) = QPoint(_position->x() + (_velocity->x() * _prediction_coeff), _position->y() + (_velocity->y() * _prediction_coeff));        
} 

void PPBot::updateForces(void){
	(*_steering) = truncate((*_correction), _max_force);
	_steering->normalize();

	// Computing acceleration
	QVector2D steeringForce(truncate((*_steering), _max_force));
	QVector2D acceleration(steeringForce);
	acceleration*=(1.0 / _mass);

	// Computing velocity
	bool braking = false;
	QPointF brakingPoint;

	for(QList<PPBot*>::const_iterator otherIter = _environment->getBots()->constBegin(); otherIter != _environment->getBots()->constEnd(); otherIter++){
		if((*otherIter) != this){
			QVector2D sepVect((*otherIter)->getPosition() - (*_position));
			double dist = sepVect.length();

			if(dist < (_radius * 2.0)){
				if(isAhead((*otherIter)->getPosition())){
					braking = true;

					QVector2D brakingVector(*_forward);
					brakingVector *= _radius;

					brakingPoint = QPointF((*_position) + brakingVector.toPointF());

					break;
				}
			}
		}
	}

	QPointF targetOffset;

	if(!braking){
		targetOffset = (*_target) - (*_position);
	}
	else{
		targetOffset = brakingPoint - (*_position);
	}

	double distance = targetOffset.manhattanLength();
	double rampedSpeed = _max_speed * (distance / _slowing_distance);

	double clippedSpeed;

	if(rampedSpeed < _max_speed){
		clippedSpeed = rampedSpeed;
	}
	else{
		clippedSpeed = _max_speed;
	}

	if(clippedSpeed < 0.01){
		_speed = 0;
	}
	else{
		_speed = clippedSpeed;
	}

	QVector2D vForce(_velocity->toPointF());
	vForce += acceleration;

	(*_velocity) = truncate(vForce, _speed);
}

void PPBot::computeCorrections(void) {
	//Obstacle avoidance
	(*_correction) = avoidSteering();

	if(_correction->length() == 0){
		//Alignment with others
		(*_correction) += (alignSteering() * 1.5);

		//Cohesion with others
		//(*_correction) += (cohesiveSteering() * 30.0);

		//Separation from others
		(*_correction) += (separativeSteering() * 50.0);

		//Target seeking
		(*_correction) += (seekSteering());
	}
}

QVector2D PPBot::seekSteering(void){
	QVector2D desiredVelocity((*_target) - (*_position));
	return (desiredVelocity - (*_velocity));
}

QVector2D PPBot::alignSteering(void){
	QVector2D alignmentSteering(0,0);

	if(_environment != 0){
		double others = 0;

		for(QList<PPBot*>::const_iterator otherIter = _environment->getBots()->constBegin(); otherIter != _environment->getBots()->constEnd(); otherIter++){
			if((*otherIter) != this){
				alignmentSteering += (*otherIter)->getForward();
				others++;
			}
		}

		if(others > 0){
			alignmentSteering *= (1.0 / others);
			alignmentSteering -= (*_forward);
		}
	}

	return alignmentSteering.normalized();
}

QVector2D PPBot::separativeSteering(void){

	QVector2D separationSteering(0,0);

	if(_environment != 0){
		double others = 0;

		for(QList<PPBot*>::const_iterator otherIter = _environment->getBots()->constBegin(); otherIter != _environment->getBots()->constEnd(); otherIter++){
			if((*otherIter) != this){
				QVector2D sepVect((*otherIter)->getPosition() - (*_position));
				double dist = sepVect.lengthSquared();

				if(sepVect.length() < (_radius * 3.0)){
					separationSteering += (sepVect * (1.0/-dist));
					others++;
				}
			}
		}

		if(others > 0){
			separationSteering /= others;
		}
	}

	return separationSteering.normalized();
}

QVector2D PPBot::cohesiveSteering(void){

	QVector2D cohesionSteering(0,0);

	if(_environment != 0){
		double others = 0;

		for(QList<PPBot*>::const_iterator otherIter = _environment->getBots()->constBegin(); otherIter != _environment->getBots()->constEnd(); otherIter++){
			if((*otherIter) != this){
				QVector2D otherPos((*otherIter)->getPosition().x(), (*otherIter)->getPosition().y());
				cohesionSteering += otherPos;
				others++;
			}
		}

		if(others > 0){
			cohesionSteering /= others;
			QVector2D pos(_position->x(), _position->y());
			cohesionSteering -= pos;
		}
	}

	return cohesionSteering.normalized();
}

QVector2D PPBot::avoidSteering(void){

	QVector2D avoidanceSteering(0,0);

	if(_environment != 0){

		double obstaclesCount = 0;

		for(QList<PPElement*>::const_iterator elemIter = _environment->getElements()->constBegin(); elemIter != _environment->getElements()->constEnd(); elemIter++){
			if((*elemIter)->shouldAvoid()){
				for(QList<QRect*>::iterator boundIter = (*elemIter)->getBounds()->begin(); boundIter!= (*elemIter)->getBounds()->end(); boundIter++){

					if(_sight->boundingRect().intersects(**boundIter)){
						// minimum distance to obstacle before avoidance is required
						double minDistanceToCollision = 120.0 * _speed;
						double minDistanceToCenter = minDistanceToCollision + ((*boundIter)->width() / 2.0);

						// contact distance: sum of radii of obstacle and vehicle
						double totalRadius = ((*boundIter)->width() / 2.0) + _radius;

						// obstacle center relative to vehicle position
						QVector2D localOffset((*boundIter)->center() - (*_position));

						// distance along vehicle's forward axis to obstacle's center
						double forwardComponent = QVector2D::dotProduct(localOffset, *_forward);
						QVector2D forwardOffset = (forwardComponent * (*_forward));

						// offset from forward axis to obstacle's center
						QVector2D offForwardOffset = localOffset - forwardOffset;

						// test to see if sphere overlaps with obstacle-free corridor
						bool inCylinder = offForwardOffset.length() < totalRadius;
						bool nearby = forwardComponent < minDistanceToCenter;
						bool inFront = forwardComponent > 0;

						// if all three conditions are met, steer away from sphere center
						if (inCylinder && nearby && inFront)
						{
							obstaclesCount++;
							avoidanceSteering += (offForwardOffset * -1.0);
						}
					}
				}
			}
		}

		if(obstaclesCount > 0){
			avoidanceSteering *= (1.0/obstaclesCount);
		}
	}

	return avoidanceSteering;
}

QVector2D PPBot::truncate(QVector2D v, double max) {
	QVector2D vT(v);
	double l = vT.length();   
	if (l > max) {     
		vT *= (max/l);    
	}
	return vT;   
} 

void PPBot::drawVector(QPainter* painter, QVector2D* v, const QColor& c, double scale) {
	painter->setBrush(c);
	painter->setPen(c);
	painter->drawLine((int) _position->x(), (int) _position->y(), (int) (_position->x() + (v->x() * scale)), (int) (_position->y() + (v->y() * scale)));
}


void PPBot::drawPoint(QPainter* painter, QPointF* p, const QColor& c, double radius) {
	painter->setBrush(c);
	painter->setPen(c);
	painter->drawEllipse((int) (p->x() - (radius / 2.0)), (int) (p->y() - (radius / 2.0)), (int) radius, (int) radius);
}

void PPBot::draw(QPainter* painter){
	//TEST
	painter->setBrush(Qt::blue);
	painter->drawEllipse((int) (_position->x() - (_radius / 2.0)), (int) (_position->y() - (_radius / 2.0)), (int) _radius, (int) _radius);

	/* Its sight rectangle*/
	painter->setBrush(Qt::red);
	painter->drawPolygon(*_sight);

	/* Its velocity */
	drawVector(painter, _velocity, QColor(255,0,255), 20.0);

	/* Its trajectory correction */
	drawVector(painter, _correction, QColor(0,255,0), 1.0);

	/* Its local space*/
	drawVector(painter, _forward, QColor(0,0,255), 20.0);
	drawVector(painter, _side, QColor(0,0,255), 20.0);

	/* Its future position and the corresponding point on the road */
	drawPoint(painter, _futurePosition, QColor(0,255,0), 8.0);
	drawPoint(painter, _target, QColor(255,0,0), 8.0);
	//END TEST
}

void PPBot::update(void){
	/* Updating forces / trajectories */
	computeCorrections();
	updateForces();
	updateFuturePosition();

	/* Updating local space */
	(*_forward) = (*_velocity);
	_forward->normalize();

	(*_side) = QVector2D((_forward->x() * cos(PI / 2.0)) - (_forward->y() * sin(PI / 2.0)), (_forward->y()  * cos(PI / 2.0)) + (_forward->x()  * sin(PI / 2.0)));
	_side->normalize();

	/* Updating position */
	(*_position) += (_velocity->toPointF());

	/* Updating sight */
	_sight->clear();
	(*_sight) << (QPointF(_position->x() + (_side->x() * (_radius / 2.0)), _position->y() + (_side->y() * (_radius / 2.0))));
	(*_sight) << (QPointF(_position->x() - (_side->x()* (_radius / 2.0)), _position->y() - (_side->y() * (_radius / 2.0))));
	(*_sight) << (QPointF(_futurePosition->x() - (_side->x() * (_radius / 2.0)), _futurePosition->y() - (_side->y() * (_radius / 2.0))));
	(*_sight) << (QPointF(_futurePosition->x() + (_side->x() * (_radius / 2.0)), _futurePosition->y() + (_side->y() * (_radius / 2.0))));
}