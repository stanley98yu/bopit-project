from flask import Flask
import math

app = Flask(__name__)
app.config['DEBUG'] = True



@app.route('/add/<first>/<second>')
def add(first, second):
	try:
		one = float(str(first))
		two = float(str(second))
		return str(one+two)
	except:
		return "ERROR"
@app.route('/subtract/<first>/<second>')
def subtract(first, second):
	try:
		one = float(str(first))
		two = float(str(second))
		return str(one-two)
	except:
		return "ERROR"
@app.route('/multiply/<first>/<second>')
def multiply(first, second):
	try:
		one = float(str(first))
		two = float(str(second))
		return str(one*two)
	except:
		return "ERROR"
@app.route('/divide/<first>/<second>')
def divide(first,second):
	try:
		one = float(str(first))
		two = float(str(second))
		return str(one/two)
	except:
		return "ERROR"


if __name__ == "__main__":
	app.run()