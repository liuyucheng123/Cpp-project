# 视频录制脚本（≤10 分钟，含中文口播）

> 目标：完整演示作业要求的每一项功能，并说明架构的可扩展性。
> 录制前：把控制台字体调大（如 18–20 pt）、窗口放大、清屏；关闭其他通知。
> 录制工具：OBS（1080p / 30 fps，窗口捕获）；成品 ≤99 MB。
> 上传后把 bilibili 链接填进报告封面与 `Projet/rapport.pdf` 的「视频链接」一行。

录制前先在 PowerShell 里调整窗口（版面按 72 列排版，窗口太窄会折断表格）：

```powershell
cd "D:\OneDrive\桌面\Cpp-project"
$Host.UI.RawUI.WindowSize  = New-Object System.Management.Automation.Host.Size(110,38)
$Host.UI.RawUI.BufferSize  = New-Object System.Management.Automation.Host.Size(110,3000)
$Host.UI.RawUI.ForegroundColor = "Gray"
Clear-Host
```

若报「窗口尺寸超出缓冲区」，先把缓冲区调大、再把字号调到 18–20 pt，最后手动拖窗口。

## 0:00–0:30 开场
- 画面：报告封面 / 项目标题。
- 口播：“我们是 LO02 A26 第 X 组，项目是 Codex Naturalis 的控制台版本，
  只用课程讲过的 C++ 与七个设计模式实现；这是我们的演示视频。”

## 0:30–1:15 设计概览（对着报告的架构图）
- 画面：报告第 6 页的四层架构图（main → controleur → vue/modele → noyau）。
- 口播：“模型层不做任何输入输出，所以既能被键盘驱动，也能被脚本和测试驱动；
  七个模式分别是 Template Method、Strategy、Factory Method、Singleton、
  Iterator、Adapter、Composite。”

## 1:15–2:00 编译与启动
```powershell
powershell -ExecutionPolicy Bypass -File scripts\build.ps1
.\build\codex_naturalis.exe
```
- 口播：“一条命令编译出可执行文件和测试；编译零警告，207 次断言全部通过。”

## 2:00–3:10 开局参数与准备
- 输入：人数 2、姓名 Alice / Bob、Bob 设为电脑（难度 3），种子 2026。
- 演示：Alice 的初始牌正反两面（四角符号 + 中央资源），选 1（recto）；
  两张候选秘密目标，选 1。
- 口播：“开局完全参数化：人数、姓名、真人或电脑、AI 难度、随机种子。”

## 3:10–5:00 回合与合法性校验
- 演示一个正常回合：选牌 → 选面 → 从程序列出的合法落点中选一个 → 抽牌。
- 故意制造三类非法输入：
  1. 手牌编号越界（输入 9）→ 提示范围；
  2. 选一张需求不满足的镀金牌的正面 → 提示「exige 3 plante 1 animal」；
  3. 在「可选落点」为空时换牌 → 提示「aucun placement possible」。
- 口播：“所有校验都在修改状态之前完成，被拒绝的动作不会留下半完成状态。”

## 5:00–6:00 镀金牌的三种计分
- 画面：抽到/打出三种镀金牌各一次，指出分数变化与原因：
  固定分（3/5）、每覆盖角 2 分、每物件 1 分（放置瞬间结算一次）。

## 6:00–6:40 抽牌与补牌
- 演示从 4 张明牌抽一张（补一张新的）与从资源堆/镀金堆抽一张。

## 6:40–7:40 终局与结算
- 触发：有人到 20 分 或 两个牌堆抽空。
- 演示：最后一回合提示 → 结果表（牌面分 / 目标分 / 总分）→ 胜者；
  说明平局时先比目标分。
- 口播：“补完本轮 + 每人最后一回合，然后加 2 张公共目标和 1 张秘密目标的分。”

## 7:40–8:40 AI 对局与三种难度
```powershell
.\build\codex_naturalis.exe --auto 3 --joueurs 4 --niveau 3 --graine 2026
```
- 口播：“同一份引擎跑 4 个 AI；niveau 1/2/3 分别是随机、贪心、目标导向。
  固定种子可复现。”

## 8:40–9:30 可扩展性（对着报告第 14 页）
- 画面：扩展表（新 AI / 新目标 / 新牌类 / 新显示 / 官方单人模式）。
- 口播：“新增 AI 只需新增一个 StrategieIA 子类，并在工厂里加一行；
  新增目标只需新增 Objectif 子类与对应工厂；规则引擎不需要改。”

## 9:30–10:00 测试与收尾
```powershell
.\build\codex_tests.exe
powershell -ExecutionPolicy Bypass -File scripts\check_scope.ps1
```
- 画面：207 次断言通过；脚本输出「Aucune fonctionnalite hors cours detectee」。
- 口播：“测试全部通过，范围检查 0 处违规，证明项目没有超出课程内容。谢谢观看。”

---

## 录制检查清单
- [ ] 视频 ≤10 分钟、≤99 MB、1080p、有中文口播
- [ ] 覆盖：开局参数化 / 放牌 / 抽牌 / 三类非法被拒 / 三种镀金计分 / 终局结算 / AI / 扩展性
- [ ] 上传 bilibili，链接填入报告封面
- [ ] 最终目录：`Projet/`（含 `rapport.pdf`、`code/`、`video/script.md`），压缩为 `Projet.zip`

---

## 附 A：命令清单（逐条实测可运行，当前共 20 个提交、207 断言全过）

```powershell
cd "D:\OneDrive\桌面\Cpp-project"                    # 项目根目录（后面所有命令都在这里跑）

powershell -ExecutionPolicy Bypass -File scripts\build.ps1   # 编译游戏+测试并跑断言
.\build\codex_naturalis.exe --aide                   # 列出全部命令行选项
.\build\codex_naturalis.exe                          # 交互对局（键盘玩）
.\build\codex_naturalis.exe --entree scripts\demo_entree.txt  # 回放一整局直到 FIN DE PARTIE
.\build\codex_naturalis.exe --auto 3 --joueurs 4 --niveau 3 --graine 2026  # 4 个 AI 连打 3 局
.\build\codex_tests.exe                              # 只跑测试（207 verifications）
powershell -ExecutionPolicy Bypass -File scripts\check_scope.ps1  # 课程范围检查
```

预期输出：
- `build.ps1` 末尾 → `207 verifications, 0 echec(s). TOUS LES TESTS PASSENT`
- `--auto` 每局一行 → `Partie 1 : scores 25 15 | cartes 25 13 | objectifs 0 2 | vainqueur 0`
  （`vainqueur 0` 是**0 基下标**，即 1 号玩家；解说时要说明，否则评阅人会以为出错）
- `check_scope.ps1` → `Aucune fonctionnalite hors cours detectee`

## 附 B：交互模式的实测按键序列

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
| 10 | `Carte a poser (1 a 3)` | `1` | 手牌编号 |
| 11 | `Face : 1 = recto, 2 = verso` | `1` | |
| 12 | `Choix du placement` | `1` | 列表第 1 个合法落点 |
| 13 | `Ou piocher ? 1-4 = carte visible, 5 = tas ressource, 6 = tas doree` | `2` | 抽第 2 张明牌 |

一个真人回合就是第 10–13 步；之后轮到电脑，屏幕会自己走完 Bob 的一回合，再回到 Alice。

三类非法输入（在任意 `Votre choix :` 处演示，输完程序会提示并重新询问）：

| 演示内容 | 键入 | 预期提示 |
|---|---|---|
| 数字越界 | 在 `Carte a poser (1 a 3)` 输 `9` | `valeur hors limites : il faut entre 1 et 3` |
| 非数字 | 输 `abc` | `ce n'est pas un nombre : abc` |
| 镀金牌需求不满足 | 选带 `exige 3 plante 1 animal` 的牌并选面 | `aucun placement possible avec cette carte et cette face` |

> 录制提示：交互模式每回合会重印整块棋盘（约 40 行），走 3–4 个回合就够，
> 不必玩到 20 分；终局效果用 `--entree scripts\demo_entree.txt` 回放更省时间，
> 回放结束时屏幕会停在 `FIN DE PARTIE` 的四列结算表上，正好对着它讲计分。
