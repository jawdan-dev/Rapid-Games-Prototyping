let canvas;
function windowResized() {
	resizeCanvas(windowWidth, windowHeight);
}
function setup() {
	canvas = createCanvas(windowWidth, windowHeight);
	canvas.position(0, 0);
	canvas.style('z-index', '-1');

	ellipseMode(RADIUS)
	noSmooth();
	strokeWeight(3);

	// for (let i = 0; i < 20000; i++) {
	// const x = map(random(), 0, 1, -boundsSize, boundsSize);
	// const y = map(random(), 0, 1, -boundsSize, boundsSize);
	// boids[boids.length] = new Boid(new Vector2(x, y));
	// }
	motherBoid = new Boid(new Vector2(0, 0), 0, true);

	structures[structures.length] = new Spawner(new Vector2(-1000, 0), 0);
	structures[structures.length] = new Spawner(new Vector2(1000, 0), 0);
	for (let i = 0; i < 40; i++) {
		const a = random() * PI * 2;
		const d = map(random(), 0, 1, 0.3, 0.95) * boundsSize;
		structures[structures.length] = new Spawner(new Vector2(sin(a) * d, cos(a) * d));
	}
}

///////////////////////////////////////////////////////

function draw() {
	background(30);
	deltaTime = deltaTime / 1000.0;
	// BOF

	handleMouseDrag();
	handleMouseZoom();

	push();
	scale(drawScale);
	translate(drawOffset.x, drawOffset.y);

	noFill();
	stroke(0, 255, 100);

	for (let i = 0; i < boids.length; i++) {
		bounds.insert(boids[i]);
	}
	if (inputManager.keyUp('`'))
		drawBounds = !drawBounds;
	if (drawBounds) bounds.draw();


	// square(100, 100, 50, 10);
	// square(150, 100, 50, 10);
	// square(200, 100, 50, 10);
	// square(100, 150, 50, 10);

	if (inputManager.mouseUp(0)) {
		motherTarget.pos = getMouseWorldPosition();
	}

	// Preprocess.
	motherBoid.process();
	for (let i = 0; i < boids.length; i++) {
		boids[i].process();
	}
	for (let i = 0; i < structures.length; i++) {
		structures[i].process(deltaTime);
	}

	// Draw structures.
	for (let i = 0; i < structures.length; i++) {
		structures[i].draw();
	}

	// Process & draw boids.
	motherBoid.finalize();
	for (let i = 0; i < boids.length; i++) {
		boids[i].finalize();
	}

	stroke(200);
	fill(255);
	{
		const color = getTeamColor(0);
		stroke(color.r, color.g, color.b);
		const targetSize = 5;
		rect(motherTarget.pos.x - targetSize, motherTarget.pos.y - targetSize, targetSize * 2, targetSize * 2);
		noFill();
		circle(motherTarget.pos.x, motherTarget.pos.y, targetSize * 2);

		const reticalOffset = 20;
		line(motherTarget.pos.x, motherTarget.pos.y - reticalOffset, motherTarget.pos.x, motherTarget.pos.y - (targetSize * 2))
		line(motherTarget.pos.x, motherTarget.pos.y + reticalOffset, motherTarget.pos.x, motherTarget.pos.y + (targetSize * 2))
		line(motherTarget.pos.x - reticalOffset, motherTarget.pos.y, motherTarget.pos.x - (targetSize * 2), motherTarget.pos.y)
		line(motherTarget.pos.x + reticalOffset, motherTarget.pos.y, motherTarget.pos.x + (targetSize * 2), motherTarget.pos.y)
	}
	fill(255);

	// Draw boids. (that are on screen....)
	const onscreen = bounds.findAllInRange(
		new Vector2(-drawOffset.x + (windowWidth / (2 * drawScale)), -drawOffset.y + (windowHeight / (2 * drawScale))),
		Math.max(windowWidth, windowHeight) / drawScale
	);
	motherBoid.draw();
	for (let i = 0; i < onscreen.length; i++) {
		onscreen[i].draw();
	}



	stroke(200);
	noFill();
	circle(0, 0, boundsSize);


	// Remove.
	for (let i = 0; i < boids.length; i++) {
		if (boids[i].alive) continue;

		boids.splice(i, 1);
		i--;
	}

	bounds.clear();

	pop();

	// EOF
	inputManager.clear();
}


let mouseDragOffset = new Vector2(0, 0);
function handleMouseDrag() {
	const mb = 1;
	if (!inputManager.mousePressed(mb)) return;
	if (inputManager.mouseDown(mb)) {
		mouseDragOffset.x = inputManager.mousePosition.x;
		mouseDragOffset.y = inputManager.mousePosition.y;
	}

	const mouseRelative = mouseDragOffset.Subtract(inputManager.mousePosition).Divide(drawScale);
	drawOffset = drawOffset.Subtract(mouseRelative);
	mouseDragOffset.x = inputManager.mousePosition.x;
	mouseDragOffset.y = inputManager.mousePosition.y;
}
function handleMouseZoom() {
	if (inputManager.mouseScroll == 0) return;

	const zoomSpeed = 0.9;

	let zoomModifier = 1.0;
	for (let i = 0; i < inputManager.mouseScroll; i++)
		zoomModifier /= zoomSpeed;
	for (let i = 0; i < -inputManager.mouseScroll; i++)
		zoomModifier *= zoomSpeed;

	const
		ox = inputManager.mousePosition.x,
		oy = inputManager.mousePosition.y;

	drawScale *= zoomModifier;
	drawOffset.x -= ((ox * zoomModifier) - ox) / drawScale;
	drawOffset.y -= ((oy * zoomModifier) - oy) / drawScale;
}