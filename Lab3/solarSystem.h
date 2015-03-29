#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

class solarSystem{
	public:
		solarSystem();
		~solarSystem();

		void render();
	private:
		void renderPlanet(float orbitrad, float orbitdeg, float size, float rotdeg);
		void renderMoon(float orbitrad, float orbitdeg, float size, float rotdeg);
};

#endif