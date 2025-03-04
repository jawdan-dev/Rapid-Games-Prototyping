class Vector2 {
	constructor(x, y) {
		this.x = x;
		this.y = y;
	}
}

Vector2.prototype.Add = function (other) {
	if (typeof other == "number")
		return new Vector2(this.x + other, this.y + other);
	return new Vector2(this.x + other.x, this.y + other.y);
}

Vector2.prototype.Subtract = function (other) {
	if (typeof other == "number")
		return new Vector2(this.x - other, this.y - other);
	return new Vector2(this.x - other.x, this.y - other.y);
}

Vector2.prototype.Multiply = function (other) {
	if (typeof other == "number")
		return new Vector2(this.x * other, this.y * other);
	return new Vector2(this.x * other.x, this.y * other.y);
}

Vector2.prototype.Divide = function (other) {
	if (typeof other == "number")
		return new Vector2(this.x / other, this.y / other);
	return new Vector2(this.x / other.x, this.y / other.y);
}

Vector2.prototype.Lerp = function (other, t) {
	const ta = 1.0 - t;
	return new Vector2(
		(this.x * ta) + (other.x * t),
		(this.y * ta) + (other.y * t)
	);
}

Vector2.prototype.SqrLength = function () {
	return (this.x * this.x) + (this.y * this.y);
}
Vector2.prototype.Length = function () {
	return Math.sqrt(this.SqrLength());
}
Vector2.prototype.Normal = function () {
	var len = this.Length();
	if (len == 0) { return new Vector2(0, 0); }
	return this.Divide(len);
}

Vector2.prototype.Dot = function (other) {
	return (this.x * other.x) + (this.y * other.y);
}
Vector2.prototype.Cross = function (other) {
	return (this.x * other.y) + (this.y * other.x);
}
Vector2.prototype.Cross = function () {
	return new Vector2(this.y, -this.x);
}

Vector2.prototype.Inverse = function () {
	return new Vector2(-this.x, -this.y);
}


Vector2.prototype.MinLimit = function (limit) {
	const len = this.Length();
	if (len >= limit) return;
	if (len <= 0) return;

	const limitLenFactor = limit / len;

	this.x *= limitLenFactor;
	this.y *= limitLenFactor;
}
Vector2.prototype.Limit = function (limit) {
	const len = this.Length();
	if (len <= limit) return;
	if (len <= 0) return;

	const limitLenFactor = limit / len;

	this.x *= limitLenFactor;
	this.y *= limitLenFactor;
}
Vector2.prototype.Clamp = function (minX, minY, maxX, maxY) {
	this.x = Math.min(Math.max(this.x, minX), maxX);
	this.y = Math.min(Math.max(this.y, minY), maxY);
}