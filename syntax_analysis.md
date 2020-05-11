# 自定义语法(基于Tiny, 风格改成C-like)
## 文法
program --> stmt-sequence

stmt-sequence --> stmt **;** {stmt **;** }

stmt --> if-stmt|for-stmt| while-stmt| assign-stmt| declare-stmt

if-stmt --> **if** **(** exp **)** **{** stmt-sequence **}**[ **else** **{** stmt-sequence **}**]

for-stmt --> **for** **(** declare-stmt **;** stmt **;** stmt  **)** **{** stmt-sequence **}**

while-stmt --> **while** **(** exp **)** **{** stmt-sequence **}**

assign-stmt --> **id** **=** exp

exp --> simple-exp [comparison-op simple-exp]

simple-exp --> term {add-op term}

term --> fator {mul-op factor}

facotr --> **(** exp **)** | **number** | **id**

comparison-op --> **<** |**<=** |**>** |**>=** |**==** |**!=**

add-op --> **+** | **-**

mul-op --> **/** | **\***

declare-stmt --> **type** **id**