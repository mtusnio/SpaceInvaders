#ifndef H_WEAPON
#define H_WEAPON

class Ship;

class Weapon
{
public:
	friend Ship;

	Weapon();
	virtual ~Weapon() { };

	
	Ship * GetOwner() { return m_Owner; };

	virtual void Fire() = 0;
private:
	void SetOwner(Ship * owner) { m_Owner = owner; };

	Ship * m_Owner;
};


#endif