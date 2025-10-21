#include "particle_ui.h"

void ParticleSystem::ParticleUI::drawUI(ParticleSystem::ParticleManager* manager)
{
	int particlesOnScreen = manager->getParticles().size();
	const char* gravityState = (gravityEnabled) ? "TRUE" : "FALSE";

	char labels[1024];
	char values[1024];
	sprintf_s(
		labels,
		"Particles\n\n"
		"Shape:\n\n"
		"Collission\n\n"
		"Size\n\n"
		"Color\n\n"
		"Life\n\n"
		"Gravity\n\n"
		"GRID\n\n"
		"FADE\n\n"
		"TRAIL\n\n"
		"BOUNDARY\n\n"
		"PRESET\n\n"
		"Apply Preset\n\n"
		"FREQUENCY\n\n"
		"EMITTER\n\n"
		"Create Emitter\n\n"
		"Create Emitter(Preset)\n\n"
		"Place Emitter\n\n"
		"Delete Emitter\n\n"
		"Delete all Emitters\n\n"
		"Toggle Emitter\n\n"
		"VelX\n\n"
		"VelY\n\n"
		"SPAWN\n\n"
		"CLEAR\n\n"
		"UI\n\n"
	);

	sprintf_s(
		values,
		"          %i \n\n"
		"  %s <A>\n\n"
		"  %s <D>\n\n"
		" %i <UP/DOWN>\n\n"
		"%s <<-/->>\n\n"
		"      %.2f <+/->\n\n"
		"      %s <Q>\n\n"
		"         %s <G>\n\n"
		"         %s <F>\n\n"
		"         %s <T>\n\n"
		"         %s <B>\n\n"
		"%s <O/P>\n\n"
		"     ENTER\n\n"
		"       %.2f <Z/X>\n\n"
		"     %s <K/L>\n\n"
		"              <R> \n\n"
		"              <E> \n\n"
		"           <LMB> \n\n"
		"           <DEL> \n\n"
		"        <BKSPC> \n\n"
		"        <LSHIFT>\n\n"
		"[%.2f, %.2f]\n\n"
		"[%.2f, %.2f]\n\n"
		"         <SPACE> \n\n"
		"             <C> \n\n"
		"           <TAB> \n\n",
		particlesOnScreen,
		shapes[shapeIndex],
		collissions[collissionIndex],
		particleSize,
		colors[colorIndex],
		particleLifetime,
		gravityState,
		(isGridEnabled) ? "ON" : "OFF",
		(fadeEffect) ? "ON" : "OFF",
		(trailEffect) ? "ON" : "OFF",
		(boundary) ? "ON" : "OFF",
		currentPresetName.c_str(),
		frequency,
		currentEmitter.c_str(),
		velocityRangeX.x, velocityRangeX.y,
		velocityRangeY.x, velocityRangeY.y
	);

	//double fontsize = WIDTH * 0.00050625 * HEIGHT * 0.00050625;
	double fontsize = double(WIDTH)/ HEIGHT * 2;
	Vector2 label_vect = MeasureTextEx(GetFontDefault(), labels, fontsize, 1);
	Vector2 value_vect = MeasureTextEx(GetFontDefault(), values, fontsize, 1);

	char fps[20];
	sprintf_s(fps, "FPS: %i", GetFPS());

	DrawRectangle(0, 0, WIDTH*0.007f + label_vect.x + value_vect.x + 30, HEIGHT, Fade(BLACK, 0.7));
	//DrawText(labels, WIDTH * 0.007f, HEIGHT * 0.05, fontsize, GOLD);
	//DrawText(values, WIDTH*0.11f, HEIGHT * 0.05, fontsize, GOLD);
	DrawText(labels, WIDTH*0.007f, HEIGHT * 0.05, fontsize, GOLD);
	DrawText(values, WIDTH*0.007f + label_vect.x + 20, HEIGHT * 0.05, fontsize, GOLD);
	DrawText(fps, WIDTH - 100, 10, 20, GREEN);
}
