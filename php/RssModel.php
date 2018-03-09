<?php
// inclusion de la classe magpierss
require_once("magpierss-0.72/rss_fetch.inc");

function FeedParser($url_feed, $nb_items_affiches=10)
{
  // lecture du fichier distant (flux XML)
  $rss = fetch_rss($url_feed);

  // si la lecture s'est bien passee,
  // on lit les elements
  if (is_array($rss->items))
  {
   // on ne recupere que les elements les + recents
   $items = array_slice($rss->items, 0, $nb_items_affiches);

   // debut de la liste
   // (vous pouvez indiquer un style CSS
   // pour la formater)
   $html = "<ul>n";

   // boucle sur tous les elements
   foreach ($items as $item)
   {
    $html .= "<li>";
    $html .= "<a href=".$item['link'].">".$item['title']."</a>";
    $html .= "</li>n";
   }
   $html .= "</ul>n";
 }

 // retourne le code HTML a inclure dans la page
 return $html;
}
?>
