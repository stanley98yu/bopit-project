from flask import Flask, render_template

app = Flask(__name__)
app.config['DEBUG'] = True

@app.route('/')
def index():
	return render_template("Me.html", nameList=["Tian","Cherie","Kevin","Alex"],numbers=[0,1,2,3], my_friends = {'kevin':70, 'will':69, 'tian':63})

if __name__ == "__main__":
	app.run()