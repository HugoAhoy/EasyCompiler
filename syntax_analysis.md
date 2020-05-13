# 自定义语法(基于Tiny, 风格改成C-like)
## 文法
program --> stmt-sequence

stmt-sequence --> block {block} 

block --> **{** stmt-sequence **}** | stmt

stmt --> if-stmt|for-stmt| while-stmt| assign-stmt **;**| declare-stmt **;**

if-stmt --> **if** **(** exp **)** block [ **else** **{** stmt-sequence **}**]

for-stmt --> **for** **(** declare-stmt **;** exp **;** assign-stmt **)** block

while-stmt --> **while** **(** exp **)** block

assign-stmt --> var **=** exp | var **=** address

address --> **&** **id**

exp --> simple-exp [comparison-op simple-exp]

simple-exp --> term {add-op term}

term --> fator {mul-op factor}

facotr --> **(** exp **)** | **number** | var

comparison-op --> **<** |**<=** |**>** |**>=** |**==** |**!=**

add-op --> **+** | **-**

mul-op --> **/** | **\***

declare-stmt --> **type** var-declare

var --> **id** {**[** exp **]**}

var-declare --> **id**[**=** exp] | {**\***} **id**| **id** **[** exp **]** {**[** exp **]**}