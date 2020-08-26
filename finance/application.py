import os
import datetime

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    rows = db.execute(
        'SELECT * FROM shares WHERE user_id = :id GROUP BY symbol', id=session['user_id'])
    cash = db.execute('SELECT cash FROM users WHERE id = :id',
                      id=session['user_id'])[0]['cash']

    for row in rows:
        row['price'] = lookup(row['symbol'])['price']

    return render_template('index.html', rows=rows, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == 'POST':
        # Ensure symbol was submitted
        symbol = request.form.get('symbol')
        if symbol == None:
            return apology('must provide a symbol', 403)

        # Perfom lookup for a symbol
        quote = lookup(request.form.get('symbol'))
        price = int(quote['price'])

        # Validate lookup result
        if quote == None:
            return apology('invalid symbol', 403)

        shares = int(request.form.get('shares'))

        # Ensure shares were submitted
        if shares == None:
            return apology('must provide shares', 403)

        # Ensure shares are positive number
        if not shares > 0:
            return apology('shares must be a positive int', 403)

        balance = db.execute('SELECT cash FROM users WHERE id = :id',
                             id=session['user_id'])[0]['cash']

        if balance < shares * price:
            return apology('not enough cash')

        stocks = db.execute('SELECT amount FROM shares WHERE user_id = :id AND symbol = :symbol',
                            id=session['user_id'], symbol=symbol)

        if not stocks:
            db.execute('INSERT INTO shares (user_id, symbol, amount) VALUES (:id, :symbol, :amount)',
                       id=session['user_id'], symbol=symbol, amount=shares)
        else:
            db.execute('UPDATE shares SET amount = :amount WHERE user_id = :id AND symbol = :symbol',
                       id=session['user_id'], symbol=symbol, amount=shares + stocks[0]['amount'])

        db.execute('UPDATE users SET cash = :cash WHERE id = :id',
                   id=session['user_id'], cash=balance-(price * shares))

        db.execute('INSERT INTO transactions (user_id, type, symbol, amount, date) VALUES (:id, :type, :symbol, :amount, :date)',
                   id=session['user_id'], type='buy', symbol=symbol, amount=shares, date=datetime.datetime.now().isoformat())

        flash('Transaction complete')
        return redirect('/')
    else:
        return render_template('buy.html')


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = db.execute(
        'SELECT * FROM transactions WHERE user_id = :id', id=session['user_id'])
    return render_template('history.html', rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == 'POST':

        # Ensure symbol was submitted
        if request.form.get('symbol') == None:
            return apology('must provide a symbol')

        # Look up a quote for a symbol
        quote = lookup(request.form.get('symbol'))

        # Validate result
        if quote == None:
            return apology('invalid symbol', 403)

        # Result valid, render template
        else:
            return render_template('quoted.html', quote=quote)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template('quote.html')


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == 'POST':

        # Ensure username was submitted
        if not request.form.get('username'):
            return apology('must provide username', 403)

        # Ensure password was submitted
        elif not request.form.get('password1'):
            return apology('must provide password', 403)

        # Ensure password confirmation was submitted
        elif not request.form.get('password2'):
            return apology('must confirm password')

        # Ensure passwords match
        elif not request.form.get('password1') == request.form.get('password2'):
            return apology('password must match')

        # Check for username uniquiness
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))
        if len(rows) != 0:
            return apology('that username has already been taken')

        # Persist user to the db
        new_user_id = db.execute('INSERT INTO users (username, hash) VALUES (:username, :hash)',
                                 username=request.form.get('username'),
                                 hash=generate_password_hash(request.form.get('password1')))

        # Check for sucessfull insertion
        if new_user_id == None:
            return apology('could not save that user to the db')

        # Remember user
        session['user_id'] = new_user_id

        # Provide feedback
        flash('Registed sucessfully')

        # Redirect user to home page
        return redirect('/')

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template('register.html')


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == 'POST':
        symbol = request.form.get('symbol')
        shares = int(request.form.get('shares'))
        price = lookup(symbol)['price']

        if symbol == None:
            apology('must provide a symbol', 403)
        if shares == None:
            apology('must provide amount')

        amount = db.execute(
            'SELECT amount FROM shares WHERE user_id = :id AND symbol = :symbol', id=session['user_id'], symbol=symbol)[0]['amount']

        new_amount = amount - shares

        if new_amount < 0:
            return apology('invalid number of shares')
        elif new_amount == 0:
            db.execute('DELETE FROM shares WHERE user_id = :id AND symbol = :symbol',
                       id=session['user_id'], symbol=symbol)
        else:
            db.execute('UPDATE shares SET amount = :amount WHERE user_id = :id AND symbol = :symbol',
                       amount=new_amount, id=session['user_id'], symbol=symbol)

        balance = db.execute(
            'SELECT cash FROM users WHERE id = :id', id=session['user_id'])[0]['cash']
        new_balance = balance + (shares * price)

        db.execute('UPDATE users SET cash = :cash WHERE id = :id',
                   cash=new_balance, id=session['user_id'])

        db.execute('INSERT INTO transactions (user_id, type, symbol, amount, date) VALUES (:id, :type, :symbol, :amount, :date)',
                   id=session['user_id'], type='sell', symbol=symbol, amount=shares, date=datetime.datetime.now().isoformat())

        flash('transaction complete')
        return redirect('/')

    else:
        rows = db.execute(
            'SELECT symbol FROM shares WHERE user_id = :id GROUP BY symbol', id=session['user_id'])
        return render_template('sell.html', rows=rows)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
