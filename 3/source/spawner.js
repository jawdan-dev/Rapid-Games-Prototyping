class Spawner {
	constructor(pos, team) {
		this.team = ((team != undefined) ? team : Math.floor(random() * (teamCount - 1)) + 1);
		this.pos = pos;

		this.timeTillNextSpawn = 0;
		this.protectionRange = 500;
		this.takeOverProgress = 0;
		this.takeOverRate = 0.1;
	}


	process(deltaTime) {

		let team0Takeover = false, hasProtection = false;
		if (this.team != 0) {
			const nearby = bounds.findAllInRange(this.pos, this.protectionRange);
			for (let i = 0; i < nearby.length && (!team0Takeover || !hasProtection); i++) {
				if (nearby[i].team == 0) {
					team0Takeover = true;
				}
				if (nearby[i].team == this.team) {
					hasProtection = true;
				}
			}

			if (team0Takeover && !hasProtection) {
				this.takeOverProgress += deltaTime * this.takeOverRate;
				if (this.takeOverProgress >= 1.0) {
					this.team = 0;
				}
			} else if (!team0Takeover && hasProtection && this.takeOverProgress > 0) {
				this.takeOverProgress -= deltaTime * this.takeOverRate * 2.0;
				if (this.takeOverProgress < 0)
					this.takeOverProgress = 0;
			}
		}


		this.timeTillNextSpawn -= deltaTime;

		if (this.timeTillNextSpawn <= 0) {
			this.spawn();
			this.timeTillNextSpawn = map(random(), 0, 1, 1, 5);

			let boidCount = boids.length;
			while (boidCount > 1000) {
				boidCount -= 1000;
				this.timeTillNextSpawn *= 3;
			}

			if (this.team != 0 && team0Takeover)
				this.timeTillNextSpawn *= 4;
		}

	}

	spawn() {
		const angle = Math.random() * 2 * Math.PI;
		const offset = new Vector2(sin(angle), cos(angle));
		boids[boids.length] = new Boid(this.pos.Add(offset), this.team, false);
	}

	draw() {
		const color = getTeamColor(this.team);
		noStroke();
		fill(color.r, color.g, color.b, 10);
		circle(this.pos.x, this.pos.y, this.protectionRange);

		if (this.team != 0) {
			const team0Color = getTeamColor(0);
			fill(team0Color.r, team0Color.g, team0Color.b, 100);
			arc(this.pos.x, this.pos.y, this.protectionRange, this.protectionRange, 0, PI * 2 * this.takeOverProgress);
		}

		stroke(255);
		fill(color.r, color.g, color.b);
		circle(this.pos.x, this.pos.y, s_boidProperties.separationRange);

		stroke(color.r, color.g, color.b);
		noFill();
		circle(this.pos.x, this.pos.y, this.protectionRange);
	}
}