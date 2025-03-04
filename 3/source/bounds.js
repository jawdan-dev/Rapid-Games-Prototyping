class Bounds {
	constructor(min, max) {
		this.min = min;
		this.max = max;
		this.mid = this.min.Add(this.max).Divide(2.0);

		this.items = [undefined, undefined, undefined, undefined];
	}

	insert(item) {
		if (this.min.x > item.pos.x || this.min.y > item.pos.y || this.max.x <= item.pos.x || this.max.y <= item.pos.y) return

		const index = ((this.mid.x > item.pos.x) ? 0 : 1) | ((this.mid.y > item.pos.y) ? 0 : 2);

		if (this.items[index] == undefined) {
			this.items[index] = item;
			return
		}

		if (Object.getPrototypeOf(this.items[index]) == Object.getPrototypeOf(this)) {
			this.items[index].insert(item);
			return;
		}

		const oldItem = this.items[index];
		switch (index) {
			case 0: { this.items[index] = new Bounds(this.min, this.mid); } break;
			case 1: { this.items[index] = new Bounds(new Vector2(this.mid.x, this.min.y), new Vector2(this.max.x, this.mid.y)); } break;
			case 2: { this.items[index] = new Bounds(new Vector2(this.min.x, this.mid.y), new Vector2(this.mid.x, this.max.y)); } break;
			case 3: { this.items[index] = new Bounds(this.mid, this.max); } break;
		}

		if (item.pos.Subtract(oldItem.pos).SqrLength() < 0.01)
			item.pos = item.pos.Add(new Vector2(random(), random()));

		this.items[index].insert(oldItem);
		this.items[index].insert(item);
	}

	findAllInRange(pos, range) {
		if (this.min.x > pos.x + range || this.min.y > pos.y + range || this.max.x <= pos.x - range || this.max.y <= pos.y - range) return [];

		let items = [];
		for (let i = 0; i < this.items.length; i++) {
			if (this.items[i] == undefined) continue;

			if (Object.getPrototypeOf(this.items[i]) == Object.getPrototypeOf(this)) {
				items = items.concat(this.items[i].findAllInRange(pos, range));
				continue;
			}

			// Precise check.
			const change = this.items[i].pos.Subtract(pos);
			if (change.SqrLength() <= range * range)
				items[items.length] = this.items[i];
		}

		return items;
	}

	clear() {
		this.items = [undefined, undefined, undefined, undefined];
	}

	draw() {
		rect(this.min.x, this.min.y, this.max.x - this.min.x, this.max.y - this.min.y);

		for (let i = 0; i < this.items.length; i++) {
			if (this.items[i] == undefined) continue;

			if (Object.getPrototypeOf(this.items[i]) == Object.getPrototypeOf(this)) {
				this.items[i].draw();
				continue;
			}

			// circle(this.items[i].pos.x, this.items[i].pos.y, 10);
		}
	}
}