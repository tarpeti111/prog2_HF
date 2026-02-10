#pragma once
#include "Invasion.h"

class EventHandler : public Invasion{
private:

	bool sickness = false;

	//Event-ek
	void AsteroidFall();
	void Marsquake();
	void HearthAttack();
	void Sickness();

public:
	EventHandler(Base*);	//gyakorlatilag default konstruktor mert semmi értelme az EventHandler-nek Base* nélkül

	bool getSickness() const { return sickness; }
	void setSickness(bool b) { sickness = b; }

	void DoRandomEvent();
	void SicknessCheck();	//Ha a beteg ember eljutott egy medbay be ahol van ágy és medic akkor meggyógyul (ha több beteg ember van a medbay-ben mint ágy amihez orvos akkor minden beteg meghal)
	void SicknessKill();	//aki a kör/év végén beteg maradt meghal
};