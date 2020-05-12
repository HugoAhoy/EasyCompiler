# 自定义语法(基于Tiny, 风格改成C-like)
## 文法
program --> stmt-sequence

stmt-sequence --> stmt **;** {block} 

block --> **{** stmt-sequence **}** | stmt

stmt --> if-stmt|for-stmt| while-stmt| assign-stmt| declare-stmt

if-stmt --> **if** **(** exp **)** block [ **else** **{** stmt-sequence **}**]

for-stmt --> **for** **(** declare-stmt **;** exp **;** stmt **)** block

while-stmt --> **while** **(** exp **)** block

assign-stmt --> **id** **=** exp **;**

exp --> simple-exp [comparison-op simple-exp]

simple-exp --> term {add-op term}

term --> fator {mul-op factor}

facotr --> **(** exp **)** | **number** | **id**

comparison-op --> **<** |**<=** |**>** |**>=** |**==** |**!=**

add-op --> **+** | **-**

mul-op --> **/** | **\***

declare-stmt --> **type** **id** **;**