import subprocess

from flask import Flask, jsonify, request
from flask_restx import Api, Resource, fields

app = Flask(__name__)
api = Api(app, version='1.0', description="API for searching")

credit_model = api.model('Credit', {
    'lower_key': fields.String(required=True, description='Lower key for the credit search'),
    'upper_key': fields.String(required=True, description='Upper key for the credit search')
})

response_model = api.model('Response', {
    'date_time': fields.String(required=True, description='Date and time of the transaction'),
    'trans_no': fields.String(required=True, description='Transaction number'),
    'credit': fields.Integer(required=True, description='Credit amount'),
    'debit': fields.Integer(required=True, description='Debit amount'),
    'detail': fields.String(required=True, description='Details of the transaction')
})

@api.route('/credit')
class Credit(Resource):
    @api.doc('get_credit')
    @api.expect(credit_model, validate=True)
    @api.response(200, 'Success', model=[response_model])
    @api.response(500, 'Internal Server Error')
    def post(self):
        data = request.json
        lower_key = data.get('lower_key')
        upper_key = data.get('upper_key')
        print(lower_key)
        print(upper_key)
        
        try:
            # Pass the parameters to the subprocess command
            result = subprocess.run(['./credit', lower_key, upper_key], capture_output=True, text=True, check=True)
            return jsonify({"output": result.stdout})
        except subprocess.CalledProcessError as e:
            return jsonify({"error": e.stderr}), 500

if __name__ == '__main__':
    app.run(debug=True)  