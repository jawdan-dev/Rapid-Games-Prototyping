class BoidProperties {
	constructor() {
		this.maxSpeed = 12.25;
		this.minSpeed = 2.0;
		this.maxForce = this.maxSpeed / 80;

		//
		this.separationStrength = 2.1;
		this.separationRange = 25;
		//
		this.alignmentStrength = 1;
		this.alignmentRange = 50;
		//
		this.cohesionStrength = 0.5;
		this.cohesionRange = 50;
		//
		this.collisionRange = 15;
		//
		this.aggressionRange = 30;
		//
		this.targetSeparationStrength = 10;
		this.targetAlignStrength = 0.00155;
	}
}
const s_boidProperties = new BoidProperties();

class Boid {
	constructor(pos, team, isMother = false) {
		this.team = ((team != undefined) ? team : Math.floor(random() * teamCount));
		this.isMother = isMother;

		const angle = Math.random() * 2 * Math.PI;
		this.acceleration = new Vector2(0, 0);
		this.velocity = new Vector2(sin(angle), cos(angle));
		this.forward = this.velocity.Normal();
		this.pos = pos;
		//
		this.alive = true;
	}

	getSeparation(boidsList, targetPos) {
		let count = 0, total = new Vector2(0, 0);

		const boundsCheck = this.pos.SqrLength();
		const boundsMax = boundsSize * boundsSize;
		if (boundsCheck > boundsMax) {
			const pastBounds = Math.sqrt(boundsCheck - boundsMax);

			total = this.pos.Normal().Multiply(-pastBounds / 100000)
			count++;
		}

		const distanceThreshold = s_boidProperties.separationRange * s_boidProperties.separationRange;

		const toTarget = targetPos.Subtract(this.pos);
		if (toTarget.SqrLength() < distanceThreshold && toTarget.SqrLength() > 0) {
			total = total.Add(toTarget.Normal().Divide(toTarget.Length()).Multiply(-s_boidProperties.targetSeparationStrength));
			count++;
		}


		let boid, difference, distance;
		for (let i = 0; i < boidsList.length; i++) {
			boid = boidsList[i];

			difference = this.pos.Subtract(boid.pos);
			distance = difference.SqrLength();
			if (distance <= 0 || distance > distanceThreshold) continue;

			const dotAngle = this.forward.Dot(boid.pos.Subtract(this.pos).Normal());
			if (boid.team != this.team && dotAngle > 0.5) continue;

			const d = difference.Normal().Divide(distance);
			total = total.Add(d);
			count++;
		}
		if (count > 1) total = total.Divide(count);

		if (total.Length() > 0) {
			total = total.Normal();
			total = total.Multiply(s_boidProperties.maxSpeed);
			total = total.Subtract(this.velocity);
			total.Limit(s_boidProperties.maxForce);
		}

		return total;
	}
	getAlignment(boidsList, targetPos) {
		let count = 0, total = new Vector2(0, 0);

		const distanceThreshold = s_boidProperties.alignmentRange * s_boidProperties.alignmentRange;
		const distanceMinimum = s_boidProperties.separationRange * s_boidProperties.separationRange;
		const aggression = s_boidProperties.aggressionRange * s_boidProperties.aggressionRange;

		const toTarget = targetPos.Subtract(this.pos);
		if (toTarget.SqrLength() > distanceMinimum) {// && toTarget.SqrLength() <= distanceThreshold * 4) {
			total = total.Add(toTarget.Multiply(s_boidProperties.alignmentStrength));
			count++;
		}

		let boid, difference, distance;
		for (let i = 0; i < boidsList.length; i++) {
			boid = boidsList[i];

			difference = this.pos.Subtract(boid.pos);
			distance = difference.SqrLength();
			if ((boid.team == this.team && distance <= distanceMinimum) || distance > distanceThreshold) continue;

			if (boid.team == this.team) {
				total = total.Add(boid.forward);
				count++;
			} else if (distance <= aggression) {
				total = total.Add(boid.pos.Subtract(this.pos).Normal());
				count++;
			}
		}

		if (count <= 0) {
			return new Vector2(0, 0);
		}

		total = total.Divide(count);
		total = total.Normal();
		total = total.Multiply(s_boidProperties.maxSpeed);
		const turn = total.Subtract(this.velocity);
		turn.Limit(s_boidProperties.maxForce);
		return turn;
	}
	getCohesion(boidsList) {
		let count = 0, total = new Vector2(0, 0);

		const distanceThreshold = s_boidProperties.cohesionRange * s_boidProperties.cohesionRange;
		const distanceMinimum = s_boidProperties.separationRange * s_boidProperties.separationRange;
		let boid, difference, distance;
		for (let i = 0; i < boidsList.length; i++) {
			boid = boidsList[i];
			if (boid.team != this.team) continue;

			difference = this.pos.Subtract(boid.pos);
			distance = difference.SqrLength();
			if (distance <= distanceMinimum || distance > distanceThreshold) continue;

			total = total.Add(boid.pos)
			count++;
		}

		if (count <= 0) {
			return new Vector2(0, 0);
		}

		total = total.Divide(count);

		let desired = total.Subtract(this.pos).Normal();
		desired = desired.Multiply(s_boidProperties.maxSpeed);
		const steer = desired.Subtract(this.velocity);
		steer.Limit(s_boidProperties.maxForce);
		return steer;
	}

	checkCollision(boidsList) {
		const collisionRangeSqr = s_boidProperties.collisionRange * s_boidProperties.collisionRange;

		let boid, difference, distance;
		for (let i = 0; i < boidsList.length; i++) {
			boid = boidsList[i];
			if (boid.team == this.team) continue;

			difference = this.pos.Subtract(boid.pos);
			distance = difference.SqrLength();
			if (distance > collisionRangeSqr) continue;

			const dotAngle = this.forward.Dot(boid.pos.Subtract(this.pos).Normal());
			if (dotAngle < 0.3) continue;

			boid.alive = false;
		}
	}

	process() {
		const maxRange = Math.max(s_boidProperties.separationRange, s_boidProperties.alignmentRange, s_boidProperties.cohesionRange, s_boidProperties.collisionRange);
		const boidsList = this.isMother ? [] : bounds.findAllInRange(this.pos, maxRange);

		let target = undefined, bestDist = Infinity;
		if (this.isMother) {
			target = motherTarget;
		} else if (this.team == 0) {
			target = motherBoid;
		} else {
			for (let i = 0; i < structures.length; i++) {
				if (structures[i].team != this.team) continue;

				const dist = structures[i].pos.Subtract(this.pos).SqrLength();
				if (dist >= bestDist) continue;

				target = structures[i];
				bestDist = dist;
			}
		}
		const targetPos = target != undefined ? target.pos : new Vector2(0, 0);

		const separation = this.getSeparation(boidsList, targetPos).Multiply(s_boidProperties.separationStrength);
		const alignment = this.getAlignment(boidsList, targetPos).Multiply(s_boidProperties.alignmentStrength);
		const cohesion = this.getCohesion(boidsList).Multiply(s_boidProperties.cohesionStrength);

		this.acceleration = this.acceleration.Add(separation);
		this.acceleration = this.acceleration.Add(alignment);
		this.acceleration = this.acceleration.Add(cohesion);

		this.checkCollision(boidsList);
	}

	finalize() {
		this.velocity = this.velocity.Add(this.acceleration);
		this.velocity.MinLimit(s_boidProperties.minSpeed);
		this.velocity.Limit(s_boidProperties.maxSpeed * (this.isMother ? 0.5 : 1));

		this.acceleration.x = 0;
		this.acceleration.y = 0;

		this.pos = this.pos.Add(this.velocity);
		this.pos.Clamp(bounds.min.x, bounds.min.y, bounds.max.x, bounds.max.y);

		this.forward = this.velocity.Normal();
	}

	draw() {
		const color = getTeamColor(this.team);
		stroke(color.r, color.g, color.b);

		let right = this.forward.Cross(),
			left = (new Vector2(0, 0)).Subtract(right);

		right = right.Subtract(this.forward);
		left = left.Subtract(this.forward);

		const drawScale = this.isMother ? 20 : 2;
		triangle(
			this.pos.x + (this.forward.x * drawScale), this.pos.y + (this.forward.y * drawScale),
			this.pos.x + (right.x * drawScale), this.pos.y + (right.y * drawScale),
			this.pos.x + (left.x * drawScale), this.pos.y + (left.y * drawScale)
		);
	}
}