#include "PPBot.h"
#include "PPEnvironment.h"

void PPBot::updateFuturePosition() {                 
	(*_futurePosition) = QPoint(_position->x() + (_velocity->x() * _prediction_coeff), _position->y() + (_velocity->y() * _prediction_coeff));        
} 

void PPBot::updateForces(void){
	(*_steering) += truncate((*_correction), _max_force);
	_steering->normalize();

	/* Computing acceleration */
	QVector2D steeringForce(truncate((*_steering), _max_force));
	QVector2D acceleration(steeringForce);
	acceleration*=(1.0 / _mass);

	/* Computing velocity */
	QVector2D vForce(_velocity->toPointF());
	vForce += acceleration;

	(*_velocity) = truncate(vForce, _speed);

}

void PPBot::computeCorrections(void) {
	(*_correction) = QVector2D(0,0);

	/* Speed corrections */
	QPointF targetOffset = (*_target) - (*_position);
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

	/* Trajectory corrections*/
	//Separation from others
	bool separate = false;

	if(_environment != 0){

		QVector2D separationForce(0,0);
		double others = 0;
		double maxForce = 0;

		for(QList<PPBot*>::const_iterator otherIter = _environment->getBots()->constBegin(); otherIter != _environment->getBots()->constEnd(); otherIter++){
			if((*otherIter) != this){
				QVector2D sepVect(_position->x() - (*otherIter)->getPosition().x(), _position->y() - (*otherIter)->getPosition().y());
				double dist = sepVect.length();
				
				if(sepVect.length() < (_radius * 4.0)){
					others++;
					sepVect.normalize();

					double force = (20.0 / dist);
					sepVect *= force;
					separationForce += sepVect;

					maxForce = (force > maxForce ? force : maxForce);
				}
			}
		}

		if(others > 0){
			separate = true;
			
			//separationForce *= (1.0 / others);
			//separationForce.normalize();
			truncate(separationForce, maxForce);

			(*_correction) += separationForce;
		}
	}

	//Target seeking
	if(!separate){
		QVector2D seek(_target->x() - _futurePosition->x(), _target->y() - _futurePosition->y());
		//seek.normalize();
		(*_correction) += seek;
	}
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
	updateForces();
	updateFuturePosition();
	computeCorrections();

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