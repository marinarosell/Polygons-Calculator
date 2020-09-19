Given that the content of `file2.txt` is

```bash
p2 1 1 0.5 0.1 0 0 1 0
p3 0.1 0.1
```

the execution of the script using the calculator on the left should produce the output on the right:

<table>
<tr>
<td>

```
# sample script for the polygon calculator
polygon p1 2 2  0 2  0 0
print p1
area p1
perimeter p1
vertices p1
centroid p1
save file1.txt p1
load file2.txt
list
print p1
print p2
print p3
union p3 p1 p2
print p3
inside p1 p3
setcol p1 1 0 0
setcol p2 0 1 0
setcol p3 0 0 1
draw image.png p1 p2 p3
bbox p4 p1 p2
print p4
polygon p5 0 0 2 2 2 0 0 2
polygon p6 1 1 3 3 3 1 1 3
intersection p7 p5 p6
setcol p5 1 0 0
setcol p6 0 0 1
setcol p7 0 1 0
draw image1.png p5 p6 p7
# some errors
foobar
print p6
```

</td>
<td>

```
#
ok
p1 0.000 0.000 0.000 2.000 2.000 2.000
2.000
6.828
3
0.667 1.333
ok
ok
p1 p2 p3
p1 0.000 0.000 0.000 2.000 2.000 2.000
p2 0.000 0.000 1.000 1.000 1.000 0.000
p3 0.100 0.100
ok
p3 0.000 0.000 0.000 2.000 2.000 2.000 1.000 0.000
yes
ok
ok
ok
ok
ok
p4 0.000 0.000 0.000 2.000 2.000 2.000 2.000 0.000
ok
ok
ok
ok
ok
ok
ok
#
error: invalid command
error: undefined polygon identifier
```

</td>
</tr>
</table>

Moreover, the content of `file1.txt` will be:

```bash
p1  0.000 0.000 0.000 2.000 2.000 2.000
```

`image.png` will be

![image.png](image.png)

and `image1.png` will be

![image1.png](image1.png)
