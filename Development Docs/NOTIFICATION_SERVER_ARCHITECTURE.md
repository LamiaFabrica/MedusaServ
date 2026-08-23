# Notification Server Architecture

## Overview
The Notification Server is a core component of MedusaServ that handles real-time delivery of alerts, messages, and updates to clients. It integrates directly with MedusaServ Professional Web Server and PsiForceDB_1.0.0, using the maintenance database for storage with the SMDU_Root user configured at install time.

## Architecture Components

### 1. Core Architecture
- **Monolithic Design**: Starting with a monolithic approach for simplicity, with potential to scale to microservices
- **Event-Driven Design**: Using an internal event system for decoupling notification producers from consumers
- **Message Queue**: Internal queue system for buffering incoming notifications
- **Dispatcher/Worker Pool**: Async workers to process queued messages
- **Push Gateways**: Integration with various notification delivery mechanisms
- **Database**: PsiForceDB for storing notifications, preferences, and delivery status

### 2. Key Features
- **Real-Time Delivery**: WebSocket connections for bidirectional communication
- **Scalability**: Horizontal scaling capabilities with load balancing
- **Reliability**: Idempotent messages with retry logic and dead-letter queues
- **Security**: JWT/OAuth authentication, encrypted payloads, rate limiting
- **Personalization**: Template engine for dynamic content
- **Analytics**: Tracking delivery rates, failures, and user engagement

### 3. Tech Stack
- **Language**: C++ (NATIVE integration with MedusaServ)
- **Protocols**: HTTP/2 for ingress, WebSockets for real-time
- **Database**: PsiForceDB with PostgreSQL compatibility
- **Deployment**: Native C++ components compiled to .so libraries

## System Components

### 1. Notification Manager
- Central component that coordinates all notification activities
- Manages notification lifecycle from creation to delivery
- Handles notification routing and prioritization

### 2. Database Integration Layer
- Interface with PsiForceDB for notification storage
- Implements connection pooling for database efficiency
- Handles data encryption for sensitive notification content

### 3. Message Queue
- Internal buffering system for notifications
- Implements priority queuing for critical notifications
- Provides persistence to prevent message loss

### 4. Dispatcher System
- Processes queued notifications using worker threads
- Implements retry logic for failed deliveries
- Manages delivery status updates

### 5. Delivery Gateways
- WebSocket gateway for real-time browser notifications
- HTTP gateway for API-based notifications
- Email gateway for traditional email notifications
- SMS gateway for text message notifications

### 6. Security Layer
- Authentication and authorization for notification access
- Encryption of notification payloads
- Rate limiting to prevent abuse

## Database Schema Design

### 1. Notifications Table
```sql
CREATE TABLE notifications (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    title VARCHAR(255) NOT NULL,
    content TEXT NOT NULL,
    type VARCHAR(50) NOT NULL, -- INFO, WARNING, ERROR, SUCCESS
    priority INTEGER DEFAULT 0, -- 0=low, 1=normal, 2=high, 3=critical
    recipient_type VARCHAR(50) NOT NULL, -- USER, GROUP, ALL
    recipient_id UUID, -- User ID or Group ID
    sender_id UUID, -- User ID of sender
    template_id UUID, -- Reference to template if used
    payload JSONB, -- Additional data for dynamic content
    scheduled_at TIMESTAMP, -- For scheduled notifications
    expires_at TIMESTAMP, -- When notification should expire
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

### 2. Notification Delivery Status Table
```sql
CREATE TABLE notification_deliveries (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    notification_id UUID REFERENCES notifications(id) ON DELETE CASCADE,
    recipient_id UUID, -- User ID
    delivery_method VARCHAR(50) NOT NULL, -- WEBSOCKET, EMAIL, SMS, HTTP
    status VARCHAR(50) NOT NULL, -- PENDING, SENT, DELIVERED, FAILED, READ
    delivered_at TIMESTAMP,
    read_at TIMESTAMP,
    failure_reason TEXT,
    retry_count INTEGER DEFAULT 0,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

### 3. Notification Preferences Table
```sql
CREATE TABLE notification_preferences (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    user_id UUID UNIQUE NOT NULL,
    email_enabled BOOLEAN DEFAULT true,
    sms_enabled BOOLEAN DEFAULT true,
    push_enabled BOOLEAN DEFAULT true,
    web_enabled BOOLEAN DEFAULT true,
    email_frequency VARCHAR(50) DEFAULT 'IMMEDIATE', -- IMMEDIATE, DAILY, WEEKLY
    sms_frequency VARCHAR(50) DEFAULT 'IMMEDIATE',
    priority_threshold INTEGER DEFAULT 1, -- Minimum priority to receive
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

### 4. Notification Templates Table
```sql
CREATE TABLE notification_templates (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    name VARCHAR(255) NOT NULL,
    description TEXT,
    subject_template TEXT, -- For email/SMS
    content_template TEXT NOT NULL, -- Template for notification content
    type VARCHAR(50) NOT NULL, -- INFO, WARNING, ERROR, SUCCESS
    priority INTEGER DEFAULT 1,
    created_by UUID, -- User ID of creator
    is_active BOOLEAN DEFAULT true,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

## Integration Points

### 1. MedusaServ Core Engine
- Integration through libmedusaserv_core_engine.so
- Event hooks for system notifications
- API endpoints for external notification requests

### 2. Security Manager
- JWT authentication for notification senders
- OAuth integration for third-party services
- Role-based access control for notification management

### 3. Performance Manager
- Monitoring for 10K+ notifications/sec throughput
- Performance metrics collection
- Bottleneck identification and optimization

### 4. PsiForceDB Connector
- User tokens management
- Preferences storage
- Delivery status tracking

### 5. Module Manager
- Hot-swapping of notification dispatcher workers
- Dynamic loading of delivery gateways
- Plugin architecture for extending functionality

## API Endpoints

### 1. Notification Management
- POST /api/v1/notifications - Create new notification
- GET /api/v1/notifications - List notifications
- GET /api/v1/notifications/{id} - Get notification details
- PUT /api/v1/notifications/{id} - Update notification
- DELETE /api/v1/notifications/{id} - Delete notification

### 2. Delivery Management
- GET /api/v1/notifications/{id}/deliveries - List delivery status
- POST /api/v1/notifications/{id}/send - Send notification immediately
- POST /api/v1/notifications/batch - Create batch notifications

### 3. User Preferences
- GET /api/v1/preferences - Get user notification preferences
- PUT /api/v1/preferences - Update user notification preferences

### 4. Templates
- POST /api/v1/templates - Create notification template
- GET /api/v1/templates - List notification templates
- GET /api/v1/templates/{id} - Get template details
- PUT /api/v1/templates/{id} - Update template
- DELETE /api/v1/templates/{id} - Delete template

## Performance Targets
- **Latency**: <100ms for 99th percentile
- **Throughput**: 10K+ notifications/sec per node
- **Reliability**: 99.9% delivery success rate
- **Scalability**: Horizontal scaling to 1M+ users

## Security Considerations
- End-to-end encryption for sensitive notifications
- JWT-based authentication for API access
- Rate limiting to prevent abuse
- Audit logging for all notification activities
- GDPR compliance for user data handling

## Monitoring and Analytics
- Real-time delivery metrics
- Failure rate tracking
- User engagement analytics
- Performance bottleneck identification
- Automated alerting for system issues
