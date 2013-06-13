(:
  Extract names - example provided by Gosse Bouma.
:)

declare function local:yield($constituent as element(node)) as xs:string
{
  let $words :=
    for $leaf in $constituent/descendant-or-self::node[@word]
      order by number($leaf/@begin)
      return $leaf/@word

  return string-join($words," ")
};

for $node in collection('corpus')/alpino_ds//node[
    (@pos="name" and not(../@cat="mwu")) or (@cat="mwu" and node[@pos="name"])]
  return
  <term>{local:yield($node)}</term>
