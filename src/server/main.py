#! /env/bin/python3

from flask import Flask, jsonify, request
import tinydb as tdb
from diffusion import Diffusion
import json


app = Flask(__name__)
db = tdb.TinyDB('db.json')
dif_table = db.table('diffusion')

@app.route("/dif/<id_>")
def dif_id(id_):
    id_ = int(id_)
    return jsonify(dif_table.get(doc_id=id_))

@app.route("/dif", methods=["GET", "POST"])
def simfs_dif():
    if request.method == "POST":
        try:
            params = request.get_json()
        except:
            err = {"error": 400, "message": "Invalid parameter json"}
            return jsonify(err), 400 
        params = Diffusion.validate(params)
        dif_table.insert(params)
    return jsonify(dif_table.get(doc_id=len(dif_table)))

@app.route("/run", methods=["GET"])
def run():
    params = dif_table.get(doc_id=len(dif_table))
    log = Diffusion.run(params)
    return jsonify(log)


if __name__ == '__main__':
    app.run()
