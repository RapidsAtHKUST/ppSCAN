# pSCAN+ Introduction

## Prune

<table>
<tr>
<th>
prune 0, <br/> defintely not directly reachable
</th>
<th>
prune 1, <br/> defintely not directly reachable
</th>
</tr>
<tr>
<td  valign="top">

```python
def is_definitely_not_reachable(self, u, v):
    du = self.inc_degree_lst[u]
    dv = self.inc_degree_lst[v]
    return True if min(du, dv) < max(du, dv) * (self.eps ** 2) else False
</pre>
```
</td>

<td  valign="top">

```python
tmp = self.compute_cn_lower_bound(i, v)
if tmp <= 2:
    self.prune1 += 1
    self.min_cn_lst[j] = PScan.direct_reachable
else:
    self.min_cn_lst[j] = tmp
```

</td>

</pre>
</td>
</tr>
</table>