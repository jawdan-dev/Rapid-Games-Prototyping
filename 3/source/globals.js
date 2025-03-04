let drawScale = 1.0;
let drawOffset = new Vector2(1920 / 2, 1080 / 2);
let drawBounds = false;

///////////////////////////////////////////////////////

const boundsSize = 10000;
const boundsBorder = 100;
const bounds = new Bounds(new Vector2(-boundsSize - boundsBorder, -boundsSize - boundsBorder), new Vector2(boundsSize + boundsBorder, boundsSize + boundsBorder));
let motherBoid = undefined;
const motherTarget = { pos: new Vector2(0, 0) };
const boids = [];
const structures = [];

const teamCount = 8;
const teamColors = [ { r: 255, g: 50, b: 50 } ];

///////////////////////////////////////////////////////

const getTeamColor = (index) => {
	while (teamColors.length <= index) {
		teamColors[teamColors.length] = {
			r: map(random(), 0, 1, 55, 255),
			g: map(random(), 0, 1, 55, 255),
			b: map(random(), 0, 1, 55, 255),
		};
	}
	return teamColors[index];
}

const getMouseWorldPosition = () => {
	return new Vector2(
		(inputManager.mousePosition.x / drawScale) - drawOffset.x,
		(inputManager.mousePosition.y / drawScale) - drawOffset.y
	);
}