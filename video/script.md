# 视频录制脚本 · 逐步键入版（≤10 分钟）

> 用法：从上到下照着念、照着敲。每一步给出三块内容——
> **【键入】** 你在 PowerShell 里敲的东西；**【屏幕】** 敲完后终端会打印什么（节选真实输出，行数截断处标 `...`）；**【口播】** 对着这块输出要讲的话。
> 总时长按 9 分 30 秒排，留 30 秒缓冲。

## 录制前 30 秒（不录）

```powershell
cd "D:\OneDrive\桌面\Cpp-project"
$Host.UI.RawUI.WindowSize  = New-Object System.Management.Automation.Host.Size(110,38)
$Host.UI.RawUI.BufferSize  = New-Object System.Management.Automation.Host.Size(110,3000)
$Host.UI.RawUI.ForegroundColor = "Gray"
Clear-Host
```

字体调到 18–20 pt，窗口拖到约 110×38（版面按 72 列排，太窄会折行）。关掉微信/QQ 通知。
录错了不要紧：游戏里按 `Ctrl+C` 就退出，重新跑一次即可（种子固定，局面完全一样）。

## 时间轴

| 段落 | 时间 | 内容 |
|---|---|---|
| 1 | 0:00–0:25 | 开场 + 报告架构图 |
| 2 | 0:25–0:55 | 编译（看编译输出） |
| 3 | 0:55–2:00 | 开局 7 个问题 + 初始牌 + 秘密目标 |
| 4 | 2:00–3:40 | 第 1 回合：读表头、选牌、选面、落点、抽牌 |
| 5 | 3:40–4:15 | 电脑回合：一行摘要 |
| 6 | 4:15–5:30 | 第 2 回合：三种非法输入被拒 |
| 7 | 5:30–6:20 | 第 2 回合：金色牌反面打出 + 即时计分 |
| 8 | 6:20–7:10 | 脚本回放到终局：结算表 |
| 9 | 7:10–8:10 | 4 个 AI 自动对局（难度与种子） |
| 10 | 8:10–9:20 | 207 个断言 + 课程范围检查 |
| 11 | 9:20–9:30 | 收尾 |

---

## 1 · 0:00–0:25 开场

**画面**：报告封面 / 第 6 页四层架构图。

**【口播】**
> 我们是 LO02 A26 第 X 组，题目是 Codex Naturalis 的控制台版本，全部用课程讲过的 C++ 与七个设计模式实现。
> 架构分四层：`noyau` 基础设施、`modele` 规则引擎、`vue` 显示、`controleur` 交互。
> 关键点是模型层不做任何输入输出，所以同一份引擎既能被键盘驱动，也能被脚本和测试驱动——下面每一步都会看到这一点。

## 2 · 0:25–0:55 编译

**【键入】**
```powershell
powershell -ExecutionPolicy Bypass -File scripts\build.ps1
```

**【屏幕】**
```text
Compilation du jeu...
Compilation des tests...
Execution des tests...
207 verifications, 0 echec(s).
TOUS LES TESTS PASSENT
Build OK : D:\OneDrive\桌面\Cpp-project\build\codex_naturalis.exe
           D:\OneDrive\桌面\Cpp-project\build\codex_tests.exe
```

**【口播】**
> 一条命令做三件事：编译游戏、编译测试、跑测试。
> 中间**没有出现任何一行 warning**——编译选项是 `-Wall -Wextra -pedantic`，也就是最严格的一档。
> 最后两行 `207 verifications, 0 echec(s)` 是自研的测试框架统计的断言条数，全部通过；测试不依赖任何第三方库。

## 3 · 0:55–2:00 开局

### 3.1 启动（0:55–1:00）

**【键入】**
```powershell
.\build\codex_naturalis.exe
```

**【屏幕】**
```text
Nombre de joueurs (1 a 4) :
Votre choix :
>
```

**【口播】**
> 交互模式。第一个问题就是人数，说明人数不是写死的常量，而是配置项。

### 3.2 七个问题（1:00–1:30）

**【键入】**（一行一个，回车确认）

```text
2          ← 人数
Alice      ← 1 号玩家名
1          ← Alice 是真人
Bob        ← 2 号玩家名
2          ← Bob 是电脑
3          ← AI 难度：3 = 目标导向
2026       ← 随机种子
```

**【屏幕】**（每答一个就打印下一个问题）
```text
Nombre de joueurs (1 a 4) :
Votre choix :
> 2
Nom du joueur 1 :
> Alice
Joueur Alice : 1 = humain, 2 = ordinateur
> 1
Nom du joueur 2 :
> Bob
Joueur Bob : 1 = humain, 2 = ordinateur
> 2
Niveau de l'ordinateur : 1 = hasard, 2 = glouton, 3 = objectifs
> 3
Graine du hasard (nombre entier positif, 2026 par defaut) :
> 2026
```

**【口播】**
> 这里五个维度都参数化了：**人数**（1–4）、**姓名**、**真人还是电脑**、**电脑难度**、**随机种子**。
> 第 5 个问题和第 6 个问题只有前面选了「电脑」才会出现——说明这不是固定问卷，而是按配置动态生成的。
> 种子固定成 2026，意味着这局牌序完全可复现：评阅人按同样的输入能拿到同样的牌。

### 3.3 初始牌选面（1:30–1:45）

**【屏幕】**
```text
--- Mise en place ---
Carte initiale de Alice : I86
      recto HG=C HD=A BG=P BD=I [-]   |   verso HG=. HD=. BG=. BD=. [plante animal champignon]
  1 = recto, 2 = verso
Votre choix :
> 1
```

**【键入】** `1`

**【口播】**
> 每人一张初始牌，正反两面同时列出来让你选。
> `HG HD BG BD` 是四个角（haut-gauche 左上、haut-droit 右上、bas-gauche 左下、bas-droit 右下），方括号里是牌中央印的资源。
> 符号约定要记住：`P A C I` 是四个王国（植物、动物、蘑菇、昆虫），`f j s` 是三种物件（羽毛、墨水瓶、手稿），
> `#` 表示这个角**不存在**（永远不能被压牌），`.` 表示角存在但没印符号（可以压牌，压了也不给资源）。
> 所以这一面：四角 C A P I、中央 `[-]` 不产资源，是典型的「四角全有」正面；
> 反面是两个空角 + 两个缺失角，中央直接产 3 种资源——这就是官方的起始牌：正面好压牌，反面好当资源。
> 我选正面 1，因为后面要靠它的四个角往上盖牌。

### 3.4 秘密目标二选一（1:45–2:00）

**【屏幕】**
```text
Objectifs proposes a Alice :
  1) [95] 2 pts par 3 symboles champignon
  2) [102] 2 pts par paire de plume
Objectif secret (1 ou 2) :
> 1
Objectifs communs :
  [93] 3 pts par L de animal
  [97] 2 pts par 3 symboles animal
```

**【键入】** `1`

**【口播】**
> 秘密目标发两张、自己留一张：这两张是同一副目标牌洗出来的，`[95]` 和 `[102]` 是目标牌的编号，方便在报告里引用。
> 下面两张 `[93] [97]` 是公共目标，两人都算分，秘密目标只算自己的。
> 注意这三张是**不同子类**的 `Objectif`（按符号数量、按 L 形、按物件对），计分逻辑各自封装，引擎只调 `evaluer(zone)`。

## 4 · 2:00–3:40 第 1 回合（Alice）

### 4.1 先读表头（2:00–2:30）

**【屏幕】**
```text
------------------------------------------------------------------------
Scores : Alice=0 Bob=0   |   Pioche : ressources 34, dorees 36
Tour 1 - Alice   etat : il faut poser une carte
Codex de Alice :
     0  1   <- x
  0  C  A
  1  P  I
```

**【口播】**
> 每回合开头这块是「局况」。
> 第一行 `Scores`：实时分数；后面的 `Pioche : ressources 34, dorees 36` 是两副牌堆还剩多少张——抽完就终局，所以这个数字是倒计时。
> 第二行 `Tour 1 - Alice`：当前是第几回合、轮到谁；回合号由引擎里的 `Partie::numeroTour()` 维护，视图只负责打印，模型层依旧不碰输入输出。
> `etat : il faut poser une carte` 是状态机给出的阶段：一回合分「放牌」和「抽牌」两段，现在在第一段。
> 下面的网格是我的 Codex：横轴 x 向右、纵轴 y 向下，初始牌永远占 (0,0)，四角各印一个符号。

### 4.2 看三张手牌（2:30–3:00）

**【屏幕】**
```text
Main de Alice :
  [1] R15 (plante)
      recto HG=# HD=I BG=f BD=P [-]   |   verso HG=. HD=. BG=. BD=. [plante]
  [2] R2 (champignon)
      recto HG=C HD=. BG=# BD=. [-]   |   verso HG=. HD=. BG=. BD=. [champignon]
  [3] D55 (plante) +2 pts par coin recouvert | exige 3 plante 1 animal
      recto HG=. HD=. BG=. BD=# [-]   |   verso HG=. HD=. BG=. BD=. [plante]
Cartes visibles au centre :
  [1] ressource R11 (plante) ...
  [2] ressource R9 (champignon) +1 pt ...
  [3] doree     D76 (insecte) +2 pts par coin recouvert | exige 1 champignon 3 insecte
  [4] doree     D66 (animal) +2 pts par coin recouvert | exige 1 plante 3 animal
```

**【口播】**
> 手牌三张，编号 `[1][2][3]` 就是等一下要输入的编号。开局规则是「两张资源牌 + 一张金色牌」，这里正好 `R15`、`R2` 是资源牌（R 开头），`D55` 是金色牌（D 开头）。
> 每张牌都打印正反两面：资源牌正面四角是符号、反面四角全空只产一个资源；金色牌多了一句 `+2 pts par coin recouvert`（每覆盖一个角加 2 分）和 `exige 3 plante 1 animal`（正面要求场上已有 3 个植物 + 1 个动物）。
> 注意 `#` 是「角不存在」、`.` 是「角存在但没符号」，两个都要和真正的资源角区分开：前者永远压不了，后者能压但压了不给资源。
> `f` 是羽毛、`j` 是墨水瓶、`s` 是手稿——三种物件用小写，四个王国用大写，一眼能看出是「资源」还是「物件」，因为目标牌有的是按王国数、有的是按物件数。
> 中央四张是公共陈列：两张资源、两张金色，抽牌时就是从这里选。

### 4.3 出牌三步（3:00–3:30）

**【键入】** `1` → `1` → `1`

**【屏幕】**
```text
Carte a poser (1 a 3) :
> 1
Face : 1 = recto, 2 = verso :
> 1
Placements possibles :
  1) (-1,-1)
  2) (1,-1)
  3) (-1,1)
  4) (1,1)
Choix du placement :
> 1
```

**【口播】**
> 出牌分三步：选牌 → 选面 → 选落点。
> 落点不是我随便填坐标，而是程序先算出**全部合法落点**再让我选：规则是「必须压住已有牌的一个可见角、不能压同一张牌的两个角、空角不能压」，初始牌只有 (0,0)，所以合法落点正好是它的四个角。
> 校验在改状态之前完成，所以列表里的每一个点都保证能落；输错了也只会被拒绝，不会留下半个状态。

### 4.4 抽牌（3:30–3:40）

**【键入】** `2`

**【屏幕】**
```text
Ou piocher ? 1-4 = carte visible, 5 = tas ressource, 6 = tas doree
Votre choix :
> 2
------------------------------------------------------------------------
Scores : Alice=0 Bob=0   |   Pioche : ressources 33, dorees 36
Tour 1 - Bob (ordinateur)   etat : il faut poser une carte
```

**【口播】**
> 第二段必然要抽牌，六选一：1–4 是四张明牌，5 是资源牌堆顶，6 是金色牌堆顶。
> 我抽第 2 张明牌。看表头变化：`Pioche` 从 `ressources 34` 变成 `33`——明牌被拿走一张后，程序会从牌堆补一张新的上来，所以明牌永远是 4 张，而牌堆少 1。
> 分数还是 `Alice=0 Bob=0`：资源牌 R15 的横幅是 `[-]`，没有即时分，得分要靠后面的金色牌和目标。

## 5 · 3:40–4:15 电脑回合

**【屏幕】**（不需要输入，程序自己走）
```text
Codex de Bob :
     0  1   <- x
  0  C  P
  1  I  A
...
Bob joue D45 en (-1,-1) verso (+2 pts)
------------------------------------------------------------------------
Scores : Alice=0 Bob=2   |   Pioche : ressources 32, dorees 36
```

**【口播】**
> 现在轮到电脑。屏幕只停一下、没有输入提示，因为它自己走完了一整个回合。
> 关键在这一行 `Bob joue D45 en (-1,-1) verso (+2 pts)`：牌号、坐标、正反面、这一手得了几分，四个信息全写出来。
> 为什么非要有这一行？因为电脑走得快，如果只看到分数从 0 跳到 2，观众会以为 AI 根本没走棋、分数是自己变的。
> 它选的是**反面**（verso）：金色牌正面要求资源，Bob 场上只有一张初始牌，不够，所以翻到反面当纯资源打——这就是规则的必然结果，不是随机。
> 表头也换了：`Tour 1 - Bob (ordinateur)`，同一个回合号，人换了。

## 6 · 4:15–5:30 第 2 回合：三种非法输入

**【口播】（进画面时说）**
> 第 2 回合我故意输错，演示校验。三种错误分别对应三层防线：格式、范围、规则。

### 6.1 越界（4:15–4:35）

**【键入】** `9`

**【屏幕】**
```text
Tour 2 - Alice   etat : il faut poser une carte
...
Carte a poser (1 a 3) :
> 9
[refuse] valeur hors limites : il faut entre 1 et 3
Carte a poser (1 a 3) :
>
```

**【口播】**
> 手牌只有 3 张，输 9 被挡回来。提示里直接写出合法区间，并且**重新问一遍**，光标回到同一个问题——这是第一层：范围校验，统一在 `demanderEntier` 里做，所有菜单共用一套提示语。

### 6.2 非数字（4:35–4:55）

**【键入】** `abc`

**【屏幕】**
```text
Carte a poser (1 a 3) :
> abc
[refuse] ce n'est pas un nombre : abc
Carte a poser (1 a 3) :
>
```

**【口播】**
> 输字母也不崩，回显你输入的原串再说明原因。这一层是流读取失败的处理：清掉输入缓冲、不让 `cin` 一直失败、也不让程序死循环。

### 6.3 规则不满足（4:55–5:30）

**【键入】** `2`（选 D55）→ `1`（试正面）

**【屏幕】**
```text
Carte a poser (1 a 3) :
> 2
Face : 1 = recto, 2 = verso :
> 1
[refuse] D55 n'est pas jouable face doree : exige 3 plante 1 animal
Carte a poser (1 a 3) :
>
```

**【口播】**
> 这一层是**规则**，不是格式。D55 的正面要求「3 个植物 + 1 个动物」，我现在场上只有 2 个植物、1 个动物，所以正面打不出去。
> 提示里把要求原文贴出来了，评阅人不用翻规则书。
> 而且注意：被拒绝之后是回到「选牌」，不是回到「选面」——因为换一张牌也可能满足要求，重来一遍最省事。
> 更重要的是，拒绝发生在**修改状态之前**：牌还在手上、分数没动、棋盘没变，这就是报告里说的「异常安全」。

## 7 · 5:30–6:20 金色牌反面打出 + 即时计分

**【键入】** `2` → `2`（改成 verso）→ `1`（第一个落点）

**【屏幕】**
```text
Carte a poser (1 a 3) :
> 2
Face : 1 = recto, 2 = verso :
> 2
Placements possibles :
  1) (0,-2)
  2) (1,-1)
  3) (-2,0)
  4) (-1,1)
  5) (1,1)
Choix du placement :
> 1
```

**【口播】**
> 同一张牌翻到反面就能打——规则里金色牌的反面是「4 个空角 + 1 个永久资源」，永远可打，但代价是这局它不再提供正面那 2 分/角的加成。
> 落点变成 5 个，因为场上已经有两张牌了，候选点变多。

**【键入】** `5`（从资源堆抽）

**【屏幕】**
```text
Ou piocher ? 1-4 = carte visible, 5 = tas ressource, 6 = tas doree
Votre choix :
> 5
------------------------------------------------------------------------
Scores : Alice=2 Bob=2   |   Pioche : ressources 31, dorees 36
Tour 2 - Bob (ordinateur)   etat : il faut poser une carte
```

**【口播】**
> 看分数：`Alice=0` 变成 `Alice=2`，两分哪来的？
> 因为 D55 的侧栏写着 `+2 pts par coin recouvert`。这个计分条件是**牌自己的属性**，跟正反面无关——反面打出去照样算。
> 这一手压住了 1 个可见角，2 × 1 = 2 分，而且是**放置瞬间结算一次**、直接加进 `score()`，终局不会重复计算。
> 这也解释了为什么结算表把 `Cartes`（即时分）和 `Objectifs`（目标分）分成两列：两套结算完全独立。
> 抽牌这次选 5，也就是资源牌堆顶。
> 看表头：`Pioche ressources` 从 **32** 变 **31**，只少 1 张——不管从明牌拿还是从牌堆顶拿，净消耗都是 1：
> 拿明牌时，牌堆要补一张上来把陈列补回 4 张；直接从牌堆顶拿就不需要补，所以两种情况都是 1 张。
> 这就是为什么两副牌堆抽空就会终局——`Pioche` 那两个数字是全局倒计时。

## 8 · 6:20–7:10 回放到终局

**【键入】**
```powershell
.\build\codex_naturalis.exe --entree scripts\demo_entree.txt
```

**【口播】**
> 手动玩到 20 分要好几分钟，所以我准备了 `--entree`：把一份按键序列存在文本文件里，程序照着回放，走完一整局。
> 这正是「模型层不做 I/O」的好处：`Controleur` 只是换了一个输入来源，引擎完全不知道对面是人还是文件。

**【屏幕】**（末尾）
```text
========================================================================
FIN DE PARTIE
   N            Joueur    Cartes   Objectifs     Total
   1             Alice         5           4         9
   2               Bob        23           2        25
Vainqueur : Bob
========================================================================
```

**【口播】**
> 结算表四列：`Cartes` 是牌面即时分，`Objectifs` 是三张目标（两张公共 + 一张秘密）换算出来的分，`Total` 是两者相加。
> Alice 只有 9 分、Bob 25 分，差在 `Cartes` 那一列：Bob 的 Codex 铺得大、压住的角多。
> 胜负规则是总分高者胜、平局先比目标分；这里 Bob 直接胜出。
> 另外 `Cartes` 和 `Objectifs` 分开列，是为了让评阅人一眼看出「即时分」和「目标分」是两套独立结算，避免有人以为总分只算牌面。

## 9 · 7:10–8:10 四个 AI 自动对局

**【键入】**
```powershell
.\build\codex_naturalis.exe --auto 2 --joueurs 4 --niveau 3 --graine 2026
```

**【屏幕】**
```text
Partie 1 : scores 27 26 22 15 | cartes 27 21 17 13 | objectifs 0 5 5 2 | vainqueur 0
Partie 2 : scores 18 24 25 11 | cartes 18 22 25 9 | objectifs 0 2 0 2 | vainqueur 2
2 partie(s) terminee(s), 62 tours joues.
```

**【口播】**
> 一次跑两局，每局一行：四个玩家的总分、牌面分、目标分、赢家。
> 这里 4 个座位全是 AI，所以不用碰键盘——一整局几十个回合，几毫秒就跑完，这也是给压力测试用的：`62 tours joues` 没有任何崩溃。
> `vainqueur 0` 是**从 0 开始的下标**，0 就是 1 号玩家，不是「零分」。这一点容易误读，所以我在报告里也注明了。
> 难度三档：`1` 随机出牌、`2` 贪心（只看眼前得分）、`3` 目标导向（评估目标牌收益）。换成 `--niveau 1` 再跑一次，同一批牌会明显打得更差。

## 10 · 8:10–9:20 测试与范围检查

**【键入】**
```powershell
.\build\codex_tests.exe
```

**【屏幕】**
```text
207 verifications, 0 echec(s).
TOUS LES TESTS PASSENT
```

**【口播】**
> 测试覆盖的是**规则**，不是界面：放置合法性、覆盖角的计数、三种目标的计分、抽牌与牌堆耗尽、异常类型是否正确抛出。
> 用自己写的极简测试框架，一个宏一行断言，所以不引入任何第三方依赖。

**【键入】**
```powershell
powershell -ExecutionPolicy Bypass -File scripts\check_scope.ps1
```

**【屏幕】**
```text
Aucune fonctionnalite hors cours detectee
```

**【口播】**
> 这个脚本扫源码，检查有没有用到课程范围之外的东西——第三方库、图形界面、网络、多线程之类，结果是一处都没有。
> 也就是说 4899 行代码全部只用课程讲过的 C++ 标准库子集。

## 11 · 9:20–9:30 收尾

**【口播】**
> 总结三句话：模型层零输入输出，所以同一份引擎能被人、脚本、测试三种方式驱动；
> 七个设计模式让扩展只需要新增子类——加一个新 AI 就是新增一个 `StrategieIA` 子类加工厂一行，加一种新目标就是新增一个 `Objectif` 子类，规则引擎一行都不用改；
> 编译零警告、207 个断言全过、范围检查零违规。谢谢观看。

---

## 附 A：命令清单（逐条实测可运行）

```powershell
cd "D:\OneDrive\桌面\Cpp-project"                    # 项目根目录（后面所有命令都在这里跑）

powershell -ExecutionPolicy Bypass -File scripts\build.ps1   # 编译游戏+测试并跑断言
.\build\codex_naturalis.exe                          # 交互对局（键盘玩）
.\build\codex_naturalis.exe --aide                   # 列出全部命令行选项
.\build\codex_naturalis.exe --entree scripts\demo_entree.txt  # 回放一整局直到 FIN DE PARTIE
.\build\codex_naturalis.exe --auto 2 --joueurs 4 --niveau 3 --graine 2026  # 4 个 AI 连打 2 局
.\build\codex_tests.exe                              # 只跑测试（207 verifications）
powershell -ExecutionPolicy Bypass -File scripts\check_scope.ps1  # 课程范围检查
```

预期输出：

- `build.ps1` 末尾 → `207 verifications, 0 echec(s). TOUS LES TESTS PASSENT`，且中间**无 warning**
- `--auto` 每局一行 → `Partie 1 : scores 27 26 22 15 | ... | vainqueur 0`
- `check_scope.ps1` → `Aucune fonctionnalite hors cours detectee`

## 附 B：交互模式完整按键表

2 名玩家，Alice 真人、Bob 电脑（难度 3），种子 2026。**按提示逐行键入**：

| # | 屏幕提示 | 键入 | 说明 |
|---|---|---|---|
| 1 | `Nombre de joueurs (1 a 4)` | `2` | |
| 2 | `Nom du joueur 1` | `Alice` | |
| 3 | `Joueur Alice : 1 = humain, 2 = ordinateur` | `1` | 真人 |
| 4 | `Nom du joueur 2` | `Bob` | |
| 5 | `Joueur Bob : 1 = humain, 2 = ordinateur` | `2` | 电脑 |
| 6 | `Niveau de l'ordinateur` | `3` | 目标导向 |
| 7 | `Graine du hasard` | `2026` | 固定种子＝可复现 |
| 8 | 初始牌 `1 = recto, 2 = verso` | `1` | 开局选面 |
| 9 | `Objectif secret (1 ou 2)` | `1` | 二选一 |
| 10 | `Carte a poser (1 a 3)` | `1` | 第 1 回合：手牌编号 |
| 11 | `Face : 1 = recto, 2 = verso` | `1` | |
| 12 | `Choix du placement` | `1` | 列表第 1 个合法落点 |
| 13 | `Ou piocher ? 1-4 = carte visible, 5 = tas ressource, 6 = tas doree` | `2` | 抽第 2 张明牌 |
| 14 | `Carte a poser (1 a 3)` | `9` → `abc` | 第 2 回合：演示前两种拒绝 |
| 15 | `Carte a poser (1 a 3)` | `2` | 选 D55 |
| 16 | `Face : 1 = recto, 2 = verso` | `1` | 演示规则拒绝（资源不足） |
| 17 | `Carte a poser (1 a 3)` | `2` | 重选 D55 |
| 18 | `Face : 1 = recto, 2 = verso` | `2` | 改反面 |
| 19 | `Choix du placement` | `1` | 落点 (0,-2) |
| 20 | `Ou piocher ?` | `5` | 从资源堆抽 |

一个真人回合就是第 10–13 步（选牌 → 选面 → 落点 → 抽牌）；之后轮到电脑，屏幕自己走完 Bob 的一回合，再回到 Alice。

三类非法输入（在任意 `Votre choix :` 处演示，输完程序会提示并重新询问）：

| 演示内容 | 键入 | 预期提示 |
|---|---|---|
| 数字越界 | 在 `Carte a poser (1 a 3)` 输 `9` | `[refuse] valeur hors limites : il faut entre 1 et 3` |
| 非数字 | 输 `abc` | `[refuse] ce n'est pas un nombre : abc` |
| 金色牌需求不满足 | 选 D55 并选 `1`（正面） | `[refuse] D55 n'est pas jouable face doree : exige 3 plante 1 animal` |

> 录制提示：交互模式每回合会重印整块棋盘（约 40 行），走 2 个回合就够，
> 不必玩到 20 分；终局效果用 `--entree scripts\demo_entree.txt` 回放更省时间，
> 回放结束时屏幕会停在 `FIN DE PARTIE` 的四列结算表上，正好对着它讲计分。

## 附 C：录制检查清单

- [ ] 视频 ≤10 分钟、≤99 MB、1080p、有中文口播
- [ ] 覆盖：编译零警告 / 开局参数化 / 放牌三步 / 抽牌六选一 / 三类非法被拒 / 金色牌即时计分 / 终局结算 / 4 AI 自动对局 / 207 断言 / 范围检查
- [ ] 讲到 `Tour N - 玩家` 表头、`Bob joue ...` 摘要行、`vainqueur 0` 是下标这三处细节
- [ ] 上传 bilibili，链接填入报告封面
- [ ] 最终目录：`Projet/`（含 `rapport.pdf`、`code/`、`video/script.md`），压缩为 `Projet.zip`
