//Pour creer ce script nous nous sommes inspiree de ceux disponible sur OPENCLASSROM
// Fonction de désactivation de l'affichage des "tooltips"

function deactivateTooltips() {


    var tooltips = document.querySelectorAll('.tooltip'),

        tooltipsLength = tooltips.length;


    for (var i = 0; i < tooltipsLength; i++) {

        tooltips[i].style.display = 'none';

    }


}



// La fonction ci-dessous permet de récupérer la "tooltip" qui correspond à notre input


function getTooltip(elements) {


    while (elements = elements.nextSibling) {

        if (elements.className === 'tooltip') {

            return elements;

        }

    }


    return false;


}



// Fonctions de vérification du formulaire, elles renvoient "true" si tout est ok


var check = {}; // On met toutes nos fonctions dans un objet littéral



check['lastName'] = function(id) {


    var name = document.getElementById(id),

        tooltipStyle = getTooltip(name).style;


    if (name.value.length >= 2) {

        name.className = 'correct';

        tooltipStyle.display = 'none';

        return true;

    } else {

        name.className = 'incorrect';

        tooltipStyle.display = 'inline-block';

        return false;

    }


};







check['firstName'] = check['lastName']; // La fonction pour le prénom est la même que celle du nom







check['login'] = function() {


    var login = document.getElementById('login'),

        tooltipStyle = getTooltip(login).style;


    if (login.value.length >= 4) {

        login.className = 'correct';

        tooltipStyle.display = 'none';

        return true;

    } else {

        login.className = 'incorrect';

        tooltipStyle.display = 'inline-block';

        return false;

    }


};

check['mail'] = function() {


    var mail = document.getElementById('mail'),

        tooltipStyle = getTooltip(mail).style;


    if (mail.value.length >= 6) {
	if(!(mail.value.indexOf("@")==-1) && !(mail.value.indexOf(".")==-1)){
		mail.className = 'correct';
		tooltipStyle.display = 'none';

		return true;
	}

    } else {

        mail.className = 'incorrect';

        tooltipStyle.display = 'inline-block';

        return false;

    }


};


check['pwd1'] = function(id) {


    var pwd1 = document.getElementById(id),

        tooltipStyle = getTooltip(pwd1).style;


    if (pwd1.value.length >= 6) {

        pwd1.className = 'correct';

        tooltipStyle.display = 'none';

        return true;

    } else {

        pwd1.className = 'incorrect';

        tooltipStyle.display = 'inline-block';

        return false;

    }


};

check['pwd1'] = check['pwd2'];







// Mise en place des événements


(function() { // Utilisation d'une IIFE pour éviter les variables globales.


    var myForm = document.getElementById('myForm1'),

        inputs = document.querySelectorAll('input[type=text]'),

        inputsLength = inputs.length;


    for (var i = 0; i < inputsLength; i++) {

        inputs[i].addEventListener('keyup', function(e) {

            check[e.target.id](e.target.id); // "e.target" représente l'input actuellement modifié

        });

    }


    myForm.addEventListener('submit', function(e) {


        var result = true;


        for (var i in check) {

            result = check[i](i) && result;

        }


        if (result) {

		myForm.submit();

        }


        e.preventDefault();


    });


    


})();

(function() { // Utilisation d'une IIFE pour éviter les variables globales.


    var myForm = document.getElementById('myForm2'),

        inputs = document.querySelectorAll('input[type=password]'),

        inputsLength = inputs.length;


    for (var i = 0; i < inputsLength; i++) {

        inputs[i].addEventListener('keyup', function(e) {

            check[e.target.id](e.target.id); // "e.target" représente l'input actuellement modifié

        });

    }


    myForm.addEventListener('submit', function(e) {


        var result = true;


        for (var i in check) {

            result = check[i](i) && result;

        }


        if (result) {

		myForm.submit();

        }


        e.preventDefault();


    });


    


})();



// Maintenant que tout est initialisé, on peut désactiver les "tooltips"


deactivateTooltips();
