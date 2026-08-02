## Why

I've always had an interest in physics, and figured combining it with programming was worth doing. This is a simulation of how objects in our solar system interact with each other, written in C++ with raylib.

## The math

**Newton's law of universal gravitation**

```
F = G * m1 * m2 / r²
```

**Euclidean distance** (Pythagorean theorem)

```
r = sqrt(dx² + dy²)
```

**Vector normalization**

```
unit = (dx / r, dy / r)
```

**Newton's second law**

```
a = F / m
```

**Newton's third law**

```
force on i = -force on j
```

**Semi-implicit Euler integration**

```
v += a * dt
p += v * dt
```
