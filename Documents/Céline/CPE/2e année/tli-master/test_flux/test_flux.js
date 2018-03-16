//Auteurs : Groupe Ecureuil
//Fichiers associés: test_flux.html
//Description : Code d'exemple donné sur internet pour mettre en place facilement un flux RSS. Cependant ce code ne fonctionnait pas car le Web Service ajax google n'est plus disponible. Nous avons pris le soin de commenter et comprendre le code.

function myGetElementsByClassName(selector) {
    if ( document.getElementsByClassName ) {    
        return document.getElementsByClassName(selector);   //Renvoie un objet de type tableau de tous les éléments séléctionnés
    }
    var returnList = new Array();   //Crée un tableau
    var nodes = document.getElementsByTagName('div');   //Renvoie une HTMLcollection des éléments avec le  nom 'div'   
                                                        //nodes sera une liste de noeuds des élément trouvés dans l'ordre dans lequel ils apparaissent dans l'arbre
                                                        //div est une chaîne représentant le nom des éléments
    var max = nodes.length; // on récupère la taille de cette liste de noeuds

    for ( var i = 0; i < max; i++ ) {   //On parcourt la liste nodes
        if ( nodes[i].className == selector ) { //className définit et obtient la valeur de l'attribut class de l'élément spécifié
            returnList[returnList.length] = nodes[i];   // on remplit le tableau returnList avec la liste nodes
        }
    }
    return returnList;
}

var rssReader = {
    containers : null,
    // initialization function
    init : function(selector) {
        containers = myGetElementsByClassName(selector);
        for(i=0;i<containers.length;i++){
            // getting necessary variables
             var rssUrl = containers[i].getAttribute('rss_url');    // prend l'attribut de rss_url dans le fichier html
            var num = containers[i].getAttribute('rss_num');        // prend l'attribut de rss_num dans le fichier html
            var id = containers[i].getAttribute('id');              // prend l'attribut de id dans le fichier html

            // creating temp scripts which will help us to transform XML (RSS) to JSON
            var url = encodeURIComponent(rssUrl);   //Encode une chaîne de texte de façon à obtenir un composant valide d'un identificateur URI (Uniform Resource Identifier: l'identifiant d'une ressource sur un réseau informatique)
            var googUrl = 'https://ajax.googleapis.com/ajax/services/feed/load?v=1.0&num='+num+'&q='+url+'&callback=rssReader.parse&context='+id;  //Web service google plus disponible

            var script = document.createElement('script');  // crée un élément HTMl de type script
            script.setAttribute('type','text/javascript');  //on spécifie ses attributs (javascript,text,charset..)
            script.setAttribute('charset','utf-8');
            script.setAttribute('src',googUrl);
            containers[i].appendChild(script);  //ajoute le document script à la fin de la liste de enfants d'un noeud parent spécifié
        }
    },

    // parsing of results by google
    parse : function(context, data) {
        var container = document.getElementById(context);   //renvoie un objet Element représentant l'élément dont la propriété id correspond à la chaîne de caractères spécifiée
        container.innerHTML = '';

        // creating list of elements
        var mainList = document.createElement('ul');// crée un élément HTMl de type ul

        // also creating its childs (subitems)
        var entries = data.feed.entries;    
        for (var i=0; i<entries.length; i++) {
            var listItem = document.createElement('li');    // crée un élément HTMl de type li
            var title = entries[i].title;   //
            var contentSnippet = entries[i].contentSnippet; 
                                                             //Un Snippet est un terme de programmation informatique désignant une petite portion réutilisable de code source ou de texte
            var contentSnippetText = document.createTextNode(contentSnippet);

            var link = document.createElement('a'); // crée un élément HTMl de type a
            link.setAttribute('href', entries[i].link);     //on spécifie ses attributs (href,target..)
            link.setAttribute('target','_blank');
            var text = document.createTextNode(title);
            link.appendChild(text); 

            // add link to list item
            listItem.appendChild(link); //ajoute le link à la fin de la liste de enfants d'un noeud parent spécifié
            var desc = document.createElement('p');     // crée un élément HTMl de type p
            desc.appendChild(contentSnippetText);   //ajoute contentSnipperText à la fin de la liste de enfants d'un noeud parent spécifié

            // add description to list item
            listItem.appendChild(desc); //ajoute desc à la fin de la liste de enfants d'un noeud parent spécifié

            // adding list item to main list
            mainList.appendChild(listItem); //ajoute le listItem à la fin de la liste de enfants d'un noeud parent spécifié
        }
        container.appendChild(mainList);    //ajoute le mainList à la fin de la liste de enfants d'un noeud parent spécifié
    }
};

window.onload = function() {
    rssReader.init('post_results');
}
