---
layout: default
title: Variables
nav_order: 3
---

# Variables
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---
## Data types

| Type         | Values                                                                                                    |
|:-------------|:----------------------------------------------------------------------------------------------------------|
| Number       | This includes both integers and floats. <br/><br/>10, 10.5                                                |
| String       | "Hello World!", 'Hello World!'                                                                            |
| Boolean      | true, false                                                                                               |
| List         | Lists can contain any value. <br/><br/>[1, 2, "hi", true, nil]                                            |
| Dictionary   | Dictionaries are key value pairs. Keys need to be of type string, nil, boolean or number. <br/><br/>{"key1": 10, 2: "two", true: "true", nil: "nil"} |
| Set          | Sets are an unordered collection of unique hashable values.
| nil          | nil                                                                                                       |

## Declaring a variable

Syntax:
```js
var someNumber = 10;
var someString = "hello";
var someOtherString = 'hello';
var someBoolean = true;
var someFalseBoolean = false;
var someNilValue = nil;
var someOtherNilValue; // Variables declared without a value are set to nil
var someList = [1, 2, "hi", 'hi', nil, true, false]; // Lists can hold any value
var someDict = {"key": 10, "key1": true, 2: [1, "2", false], true: {"1": 2}};
var someSet = set();
```


To define a variable in Dictu the syntax is as follows `var <identifier> = <value>;` where identifier
is defined as such `[a-zA-Z_]+` and value is a valid datatype.

Once a variable has been defined, `var` is no longer needed to update the value of the variable.

E.g
```js
var someNumber = 10;

someNumber = someNumber + 1;
```

Variables are also allowed to change their type at runtime.

E.g
```js
var someVariable = 10;
someVariable = "Some text"; // Is perfectly valid
```

You can also define multiple variables with a single statement.

```js
var a, b = 10, c = "hello!";
print(a, b, c); // nil, 10, 'hello!'
```

## Casting

### value.toString()

You can convert values into strings with the toString() method.

```js
true.toString();
nil.toString();
1.toString();
someDict.toString();
someList.toString();
someSet.toString();
```

### value.toBool()

You can convert values into booleans with the toBool method. This is based on whether the
value is "truthy" or not. A "truthy" value is a value which has a length greater than 0
or is not 0 when checking number types. `nil` is always false.

```js
"".toBool(); // false
"test".toBool(); // true
"false".toBool(); // true

0.toBool(); // false
10.toBool(); // true

[].toBool(); // false
[1].toBool(); // true
[[]].toBool(); // true
```
