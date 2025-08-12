# Tree isomorphism classes

In this case study we look at how we can improve the performance of a simple program
that (1) generates free trees uniformly at random and then (2) stores the unique
trees up to graph isomorphism. This can be done easily with a vector, storing in
it the new tree when it is not isomorphic to any of the trees already present in
the vector. But this is quadratic in the number of unique trees that are to be stored
in the vector.

```c++
std::vector<tree> unique;
for (int i = 0; i < N; ++i) {
	tree new = generate_tree();
	
	bool exists = false;
	for (int j = 0; j < unique.size(); ++j) {
		if (isomorphic(new, unique[j])) {
			exists = true;
			break;
		}
	}
	
	if (not exists) {
		unique.push_back(tree);
	}
}
```

## Speeding this up

To speed this up, we could group the trees using specific features, numeric or
otherwise, so that we only need to check for isomorphism between the new tree and
those in the group with the same value of that specific feature. This is correct
for those features such that

- if two trees are isomorphic, then the value of the feature is the same,
- if the value is different for some pair of trees, then these are not isomorphic.

Needless to say that the feature having the same value for any pair of trees does
not necessarily imply that the trees are isomorphic.

```c++
std::vector< group<int, tree> > classes;
for (int i = 0; i < N; ++i) {
	tree new = generate_tree();
	
	// assume, w.l.o.g., that the feature is an integer value
	int f = feature(tree);
	
	// if the group was not found, method 'find' could create a new group.
	group<tree>& g = find(classes, f);
	
	bool exists = false;
	for (int j = 0; j < g.size(); ++j) {
		if (isomorphic(new, g[j])) {
			exists = true;
			break;
		}
	}
	if (not exists) {
		g.push_back(tree);
	}
}
```

These "groups" are, essentially, equivalence classes of trees but not up to graph
isomorphism, but up to equality of the feature value.

## Using the classification tree

The classification tree in this repository is perfectly suitable for this task.
Each level of the tree classifies the trees using a different feature without much
effort in nesting deeper layers of features.

This is in contrast with what we can do with arrays. When using a single feature
(as in the illustrative example above) it is sort of easy to sort the vector to
search the equivalence classes quickly. However, this task becomes more difficult
when we want to nest groups within groups, each level having its own feature.

## Example of features

An example of such a feature is the sum of squared degrees (yes, the sum of degrees
is the same for all $n$-vertex trees; recall the handshaking lemma).

### The features used in this case study

The features applied in this study case are:

- the minimum sum of edge lengths in [planar arrangements](https://www.sciencedirect.com/science/article/pii/S0020019021001198) of trees,
- the expected value of the [number of crossings](https://arxiv.org/pdf/2003.03258v3) of (uniformly) random arrangements of trees,
- the variance of the [number of crossings](https://arxiv.org/pdf/2003.03258v3) of (uniformly) random arrangements of trees.

Using these features, one can obtain speed up values up to 100x.

## Running the examples

After compiling the sources (you will need to have the [Linear Arrangement Library](https://github.com/LAL-project/linear-arrangement-library)
installed), run the `profiling` program to see the effects of the speed up. This
the usage message:

```
Exactly 3 arguments are needed.
    n: number of vertices
    N: number of trees to generate
    t: test to profile
        0
        1_Dminpl
        2_Dminpl_Cexp
        2_Dminpl_Cvar
        3_Dminpl_Cexp_Cvar
```

You can run the program as simply as:

```
$ ./trees_profiling 100 10000 2_Dminpl_Cvar
```

## Some numbers

### 15 vertices

#### No features

Store all unique $15$-vertex trees among those 10'000 trees generated using no
features at all:

```
A.0)
n	N		time		unique
15	7000	626.9	4575
15	8000	654.474	4969
15	9000	702.876	5316
15	10000	702.882	5582
```

The execution time in one row is the time needed to add the amount of trees to go
from the row above to that row, given in milliseconds. `702.9` milliseconds is not
that much, is it?

#### One feature: the minimum planar sum of edge lengths

Now, let's try using just one feature, the minimum of sum edge lengths in planar
arrangements:

```
A.1)
n	N		time		unique
15	7000	122.76		4575
15	8000	129.601		4969
15	9000	138.738		5316
15	10000	141.943		5582
```

The execution time now also measures the time needed to calculate the features
used to classify the trees. Notice that the execution time has been divided
approximately by $5$ from $A.0$ to $A.1$. Using more features the situation
does not improve much more.

### 20 vertices

But the above is not very impressive, let's look at what happens if we increase
the size of the trees to $20$ vertices.

#### No features

When using no features at all, we get:

```
B.0)
n	N		time		unique
20	7000	1336.2		6969
20	8000	1579.58		7961
20	9000	1838.99		8944
20	10000	2063.48		9932
```

The execution time has increased by, approximately, a factor of $3$ with respect
to $A.0$.

#### One feature: the minimum planar sum of edge lengths

When using the same single feature we get:

```
B.1)
n	N		time		unique
20	7000	160.05		6969
20	8000	182.69		7961
20	9000	220.459		8944
20	10000	240.633		9932
```

The speed up factor is almost $10$ from $B.0$ to $B.1$. But here we can use more features.

#### Two features: the minimum planar sum of edge lengths and the expected number of edge crossings

When using option `2_Dminpl_Cexp` of the profiler:

```
B.2)
n	N		time		unique
20	7000	108.953		6969
20	8000	126.401		7961
20	9000	151.888		8944
20	10000	162.069		9932
```

We get a speedup factor of about $1.5$ from $B.1$ to $B.2$.

#### Two features: the minimum planar sum of edge lengths and the variance of the number of edge crossings

Now, this is a speed up factor higher than $10$. Selecting a more appropriate
second feature, the variance of edge crossings in uniformly random arrangements,
accessed via option $2$ we get:

```
B.3)
n	N		time		unique
20	7000	7.42983		6969
20	8000	7.45234		7961
20	9000	7.47187		8944
20	10000	7.7867		9932
```

Now, this is what I call a speed up factor.

### 100 vertices

Notice that since the amount of unique $100$-vertex (free) trees up to graph
isomorphism is gigantic, we should not expect finding repeated trees when
generating at most 10'000 trees.

#### No features

```
C.0)
n	N		time		unique
100	7000	3154.04		7000
100	8000	3652.79		8000
100	9000	4129.44		9000
100	10000	4627.05		10000
```

#### One feature: the minimum planar sum of edge lengths

```
C.1)
n	N		time		unique
100	7000	86.5208		7000
100	8000	97.9543		8000
100	9000	108.36		9000
100	10000	123.245		10000
```

Yes, a speed up factor of approximately $35$.

#### Two features: the minimum planar sum of edge lengths and the expected number of edge crossings

```
C.1)
n	N		time		unique
100	7000	26.1793		7000
100	8000	27.19		8000
100	9000	27.6609		9000
100	10000	30.2547		10000
```

The speed up factor is now around $150$.

#### Two features: the minimum planar sum of edge lengths and the variance of the number of edge crossings

```
C.2)
n	N		time		unique
100	7000	22.2746		7000
100	8000	22.3222		8000
100	9000	22.1965		9000
100	10000	22.0889		10000
```

That is even nicer speed up factor around $200$.

#### All features

Using all features does not really improve the execution time much.




