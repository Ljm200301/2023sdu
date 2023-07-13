# 比较Firefox和谷歌的记住密码插件的实现区别
## 谷歌记住密码插件
- 记住的密码不会因为修改表单属性而消失，即使其他同域名下的表单属性发生改变，记住的密码依然会显示。
- 在显示表单数据时，按照先name后id的规则显示，同name或同id的input先取相同name的记住值，后取相同id的记住值。  
如被记住的input为
```
<input type="text" name="user1" id="user1" >
<input type="text" name="user2" id="user2" >
```
现其他页面有input如下：
```
<input type="text" name="user1" id="user2" > //显示的是上面第一个，既name 为 user1， id 为user1 的input记住值
<input type="text" name="user2" id="user1" > //显示的是上面第二个，既name 为 user2， id 为user2 的input记住值
```

- 对于type=password的input，Chrome不会弹出记住密码的弹窗，只有在跳转的下一个页面有password的input时才会自动填充密码。输入框内容是共享的，不受name和id的影响。
- type=password的DOM上面最近的input默认被Chrome认为是password的用户名，记住密码的原理是按照域名+user+password的形式记住的。
- 即使主表单是被记住的，如果主表单没有name或id，后期依然不能显示，但被记住的密码在其他同站点下表单同类型的input会显示。
如 主表单 index.html 即便只有user 没有name 和id ，password框有name，也会如此
```
<input type="text" placeholder="user"> //记住后不显示
<input type="Password" name="psd" placeholder="Password"> //记住后不显示
```

附表单 password.html
```
<input type="password" name="passworpooo" id="r" class="form-control" placeholder="456446">//记住后显示
```

- 每次修改代码需要清空所有缓存及记住的密码，否则会出现修改了代码但依然显示记住的密码的情况。
- 类型不同但name或id相同的表单域也会显示记住的值，例如email类型的输入框实际上可能是text类型，在自动填充时会按照name或id的匹配来填充值。
如：主表单 index.html
```
<input type="email" name="user" placeholder="Email">
<input type="text" name="user1231" placeholder="user">
<input type="Password" name="psw1231" placeholder="Password">
```
附表单 email.html
```
<input type="text" name="user"> //显示
<input type="email" name="user" id="Email2">//显示
<input type="email" id="user">//显示
```
当主表单被记住后， email.html 中第一个input双击后，依然显示了主表单记住的内容
- 记住密码不同于其他输入域的自动填充，没有弹窗选择记住密码，而是自动记住并隐藏，不会显示在表单中。

  
## Firefox记住密码插件
Firefox 通过`form.elements[n].type == "password"`（遍历所有表单元素）来检测密码，然后通过在表单元素中向后搜索密码字段之前的文本字段来检测用户名字段。  
登录表单必须是 a 的一部分，设置`id="password"`密码字段可能也不会造成什么影响。
#### 密码管理器的工作原理
密码管理器的主要功能是保存和填写用户的登录信息，以便用户在访问网站时能够快速登录。但是，由于不同网站的表单和URL可能存在差异，密码管理器需要猜测填写或保存用户名和密码的方法。
#### 填写登录信息
密码管理器会遍历页面上的每个表单元素，查找用户名和密码，并尽可能地匹配保存的登录信息。为了确定用户名和密码，密码管理器搜索第一个输入字段作为用户名，搜索第一个类型为"password"的输入字段作为密码。HTML5新增了一些输入类型，如`"type=email"`，因此密码管理器也会查找这些类型的字段作为用户名。
#### 保存登录信息
密码管理器会监视表单提交事件，并确定表单中的用户名和密码。如果用户名是新的，则会询问用户是否要保存该新登录信息。如果用户名已存在，但密码不同，则会询问用户是否要更改已保存的密码。如果用户名和密码已经保存，则不需要做任何操作。但是，由于表单可能具有不同的字段顺序和输入类型，密码管理器还需要考虑如何确定哪些字段表示用户名和密码。
#### 处理URL差异
不同网站的URL可能会导致填充或保存失败，因此密码管理器还需要处理URL的差异。例如，密码管理器会根据URL的协议来决定是否填充或保存登录信息。如果一个网站同时提供HTTP和HTTPS访问，那么密码管理器会将它们视为两个不同的网站，并分别保存登录信息。此外，密码管理器还会使用表单的action URL来确定登录信息的保存位置。
总之，Firefox密码管理器通过不断尝试和调整，以尽可能适应各种情况，并提供方便的登录信息管理功能。


# 参考链接
https://support.mozilla.org/zh-CN/kb/password-manager-remember-delete-change-passwords  
https://stackoverflow.com/questions/2398763/how-does-browser-know-when-to-prompt-user-to-save-password
https://segmentfault.com/a/1190000006914652
