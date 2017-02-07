/**
* Attaches an event to object
* @toObj - DOM object we are attaching an event to
* @typeEvt - the type of an event
* @func - a function to run when the event happened
**/
function addEventToObj (toObj, typeEvt, func) {
	if(document.addEventListener){
		toObj.addEventListener(typeEvt, func, false);
	}
	else if (document.attachEvent) {          	//if addEventListener isn't supported
		toObj.attachEvent("on"+typeEvt, func); 
	} else {
		toObj["on"+typeEvt] = func;				//if not able to add addEventListener or attachEvent, add the event directly to event property	
	}
};

addEventToObj(window, "load", checkForm);
addEventToObj(window, "change", checkForm);
addEventToObj(window, "keyup", checkForm);
/**
  * Checks completion of all form inputs and enables submission button
**/
function checkForm() {
	var htmlForm = document.getElementById("form");
	var button = Form.getSubmit(htmlForm.id);
	button.disabled = true;

	var inputsCorrect = checkInputs();

	if (inputsCorrect) {
		button.disabled = false;
		if (button.className === "enabled-button")
			button.style.opacity = "1";
	} else {
		button.disabled = true;
		if (button.className === "enabled-button")
			button.style.opacity = "0.5";
	}

	/**
  	* Checks completion of inputs filling and returns boolean value (which is used for submission button activating afterwards)
	**/
	function checkInputs() {
		var inputs = htmlForm.getElementsByTagName("input");

		var firstName = firstNameVal();
		var secondName = secondNameVal();
		var email = emailVal();
		var phone = phoneVal();
		var card = cardVal(); 

		if (firstName && secondName && email && phone && card) 
			return true;
		else
			return false;

	    /**
  		* Checks completion of particular inputs
		**/
	    function firstNameVal() {
			if ((Form.validateLength(inputs[0], Form.firstname)) && Form.validateWord(inputs[0])) 
				return true;
			else 
				return false;
		};	

		function secondNameVal() {
			if ((Form.validateLength(inputs[1], Form.surname)) && Form.validateWord(inputs[1]))
				return true;
			else 
				return false;
		};

		function emailVal() {
			if (Form.validateEmail(inputs[2])) 
				return true;
			else
				return false;
		};

		function phoneVal() {
			if ((Form.validateLength(inputs[3], Form.phone)) && Form.validatePhone(inputs[3])) 
				return true;
			else
				return false;

		};

		function cardVal() {
			if ((Form.validateLength(inputs[4], Form.card)) && Form.validateDigit(inputs[4])) 
				return true;
			else
				return false;
		};
	};

	addEventToObj(htmlForm, "change", checkInputs);        
	addEventToObj(htmlForm, "keyup", checkInputs);
	addEventToObj(htmlForm, "submit", checkInputs);
         
};

 /**
   * Creation of an object "Form" with properties and functions for validation
 **/
var Form = {
	validClass : " valid",

	invalidClass : " invalid",

	firstname : {
		minLength : 2,
		maxLength : 10,
	},

	surname : {
		minLength : 2,
		maxLength : 10,
	},

	phone : {
		minLength : 12,
		maxLength : 16,
	},

	card : {
		minLength : 12,
	},

	/**
    * Checks if the minimum (maximum) length is as defined in properties
    * @inputElement - DOM element to validate
    * @property - type of an object property
    **/
	validateLength : function(inputElement, property) {
		if (inputElement.value.length < property.minLength || inputElement.value.length > property.maxLength) {
			if (inputElement.className.indexOf("empty") !== -1) {
				inputElement.className = inputElement.className.replace("empty", Form.invalidClass);
			} else {
				inputElement.className = inputElement.className.replace(Form.validClass, Form.invalidClass);
			}
			return false;
		} else {
			if(inputElement.className.indexOf(Form.validClass) == -1)
			inputElement.className = inputElement.className.replace(Form.invalidClass, Form.validClass);
			return true;
		}
	},

	/**
    * Allows to input just letters without spaces
    * @inputElement - DOM element to validate
    **/
    validateWord : function(inputElement) {
    	var regExp = /^[a-zA-Z]*$/i;
    	var wordTest = regExp.test(inputElement.value);
    	if (wordTest) {
			if (inputElement.className.indexOf(Form.validClass) == -1) 
			inputElement.className = inputElement.className.replace(Form.invalidClass, Form.validClass);
			return true;
		} else {
			if (inputElement.className.indexOf("empty") !== -1) {
				inputElement.className = inputElement.className.replace("empty", Form.invalidClass);
			} else {
				inputElement.className = inputElement.className.replace(Form.validClass, Form.invalidClass);
			}
			return false;
		}
    },

	/**
    * Checks if the email is in defined format (letters, digits, @, .)
    * @inputElement - DOM element to validate
    **/
	validateEmail : function(inputElement) {
		var regExp = /^([0-9a-zA-Z]([-.\w]*[0-9a-zA-Z])*@([0-9a-zA-Z][-\w]*[0-9a-zA-Z]\.)+[a-zA-Z]{2,9})$/;
		var emailTest = regExp.test(inputElement.value);
		if (emailTest) {
			if (inputElement.className.indexOf(Form.validClass) == -1) 
			inputElement.className = inputElement.className.replace(Form.invalidClass, Form.validClass);
			return true;
		} else {
			if (inputElement.className.indexOf("empty") !== -1) {
				inputElement.className = inputElement.className.replace("empty", Form.invalidClass);
			} else {
				inputElement.className = inputElement.className.replace(Form.validClass, Form.invalidClass);
			}
			return false;
		}
	},

	/**
    * Checks if the phone number is in defined format (digits, +, (), -)
    * @inputElement - DOM element to validate
    **/
	validatePhone : function(inputElement) {
		var regExp = /^[+]*[(]{0,1}[0-9]{1,4}[)]{0,1}[-\s\./0-9]*$/;
		var phoneTest = regExp.test(inputElement.value);
		if (phoneTest) {
			if (inputElement.className.indexOf(Form.validClass) == -1) 
			inputElement.className = inputElement.className.replace(Form.invalidClass, Form.validClass);
			return true;
		} else {
			if (inputElement.className.indexOf("empty") !== -1) {
				inputElement.className = inputElement.className.replace("empty", Form.invalidClass);
			} else {
				inputElement.className = inputElement.className.replace(Form.validClass, Form.invalidClass);
			}
			return false;
		}

	},

	/**
    * Allows to input just digits without any other symbols like '-'
    * @inputElement - DOM element to validate
    **/
	validateDigit : function(inputElement) {
		var regExp = /^[{0,1}[0-9]{1,4}]{0,1}[\s\./0-9]*$/;
		var digitTest = regExp.test(inputElement.value);
		if (digitTest) {
			if (inputElement.className.indexOf(Form.validClass) == -1) 
			inputElement.className = inputElement.className.replace(Form.invalidClass, Form.validClass);
			return true;
		} else {
			if (inputElement.className.indexOf("empty") !== -1) {
				inputElement.className = inputElement.className.replace("empty", Form.invalidClass);
			} else {
				inputElement.className = inputElement.className.replace(Form.validClass, Form.invalidClass);
			}
			return false;
		}
	},

	/**
    * Searches for a submit button in a given form
    * @formID - id attribute of a given form 
    **/
	getSubmit : function(formID) {
		var inputs = document.getElementById(formID).getElementsByTagName("button");
        for(var i = 0; i < inputs.length; i++){
            if(inputs[i].type == "submit"){    
                return inputs[i];
            }       
        }       
        return false;
	}
};